#ifndef S21_MULTISET
#define S21_MULTISET

#include "../part_0/basic_tree.h"
#include <vector>

namespace s21 {

template <typename T>
class multiset : public BasicTree<std::pair<T, size_t>> {
 private:
  using Parrent = BasicTree<std::pair<T, size_t>>;
  using Parrent::root_;
  using Parrent::size_;
  using Parrent::zero_;
  using typename Parrent::TreeNode;

 public:
  class MultisetIterator : public Parrent::TreeIterator {
   public:
    TreeNode* current_{nullptr};
    multiset* owner_{nullptr};
    MultisetIterator() {}
    MultisetIterator(TreeNode* current, multiset* owner) {
      current_ = current;
      owner_ = owner;
    }
    MultisetIterator(const MultisetIterator& other) = default;
    MultisetIterator(MultisetIterator&& other) = default;

    MultisetIterator& operator=(MultisetIterator other) {
      current_ = other.current_;
      owner_ = other.owner_;
      return *this;
    }
    MultisetIterator leftiest() {
      if (!current_) current_ = owner_->root_;
      while (current_ && current_->l) current_ = current_->l;
      return *this;
    }
    MultisetIterator rightiest() {
      if (!current_) current_ = owner_->root_;
      while (current_ && current_->r) current_ = current_->r;
      return *this;
    }
    MultisetIterator& operator++() {
      if (!current_)
        *this = leftiest();
      else if (current_->r) {
        current_ = current_->r;
        *this = leftiest();
      } else {
        while (current_->p && current_ == current_->p->r)
          current_ = current_->p;
        current_ = current_->p;
      }
      return *this;
    }
    MultisetIterator operator++(int) {
      MultisetIterator tmp = *this;
      ++(*this);
      return tmp;
    }
    MultisetIterator& operator--() {
      if (!current_)
        *this = rightiest();
      else if (current_->l) {
        current_ = current_->l;
        *this = rightiest();
      } else {
        while (current_->p && current_ == current_->p->l)
          current_ = current_->p;
        current_ = current_->p;
      }
      return *this;
    }
    MultisetIterator operator--(int) {
      MultisetIterator tmp = *this;
      --(*this);
      return tmp;
    }
    T& operator*() {
      if (!this->current_) return this->owner_->zero_.first;
      return this->current_->data_.first;
    }
    bool operator==(MultisetIterator other) {
      return current_ == other.current_;
    }
    bool operator!=(MultisetIterator other) {
      return current_ != other.current_;
    }
  };
  class MultisetConstIterator : public MultisetIterator {
   public:
    using MultisetIterator::MultisetIterator;
    const T& operator*() {
      if (!this->current_) return this->owner_->zero_.first;
      return this->current_->data_.first;
    }
  };

  using key_type = T;
  using value_type = T;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = size_t;
  using iterator = MultisetIterator;
  using const_iterator = MultisetConstIterator;
  using node = TreeNode;
  using pair_t = std::pair<T, size_t>;

  multiset() {}
  multiset(std::initializer_list<value_type> const& items) : multiset() {
    for (auto n : items) this->insert(n);
  }
  multiset(multiset& other) : multiset() {
    for (auto i = other.begin(); i != other.end(); ++i) this->insert(*i);
  }
  multiset(multiset&& other) : multiset() {
    root_ = other.root_;
    size_ = other.size_;
    other.root_ = nullptr;
    other.size_ = 0;
  }
  multiset& operator=(multiset&& other) {
    root_ = other.root_;
    size_ = other.size_;
    other.root_ = nullptr;
    other.size_ = 0;
    return *this;
  }

  using Parrent::clear;
  using Parrent::empty;
  using Parrent::max_size;
  using Parrent::size;
  using Parrent::swap;

  iterator begin() { return iterator(root_, this).leftiest(); }
  iterator end() { return iterator(nullptr, this); }

  iterator insert(const_reference value) {
    size_t id{};
    pair_t new_content(value, id);
    node** pos = Parrent::pos_search(new_content, &root_);
    if (size_ == 0) {
      root_ = new node(new_content);
    } else {
      while (new_content == (*pos)->data_) {
        new_content.second += 1;
        pos = Parrent::pos_search(new_content, pos);
      }
      if (new_content > (*pos)->data_) {
        (*pos)->r = new node(new_content, *pos);
        pos = &(*pos)->r;
      } else if (new_content < (*pos)->data_) {
        (*pos)->l = new node(new_content, *pos);
        pos = &(*pos)->l;
      }
    }
    size_++;
    return iterator(*pos, this);
  }
  void merge(multiset& other) {
    for (auto i = other.begin(); i != end(); ++i) insert(*i);
  }
  iterator find(const_reference key) {
    iterator result = end();
    node** pos = Parrent::pos_search(pair_t(key, 0), &root_);
    if ((*pos)->data_.first == key) result = iterator(*pos, this);
    return result;
  }
  size_type count(const_reference key) {
    size_type result{};
    iterator i = find(key);
    while (i != end()) {
      result++;
      i++;
    }
    return result;
  }
  std::pair<iterator, iterator> equal_range(const_reference key) {
    iterator start = find(key);
    iterator i = start;
    while (i != end() && *i == key) i++;
    return std::pair<iterator, iterator>(start, i);
  }
  bool contains(const_reference value) {
    iterator pos = find(value);
    return pos.current_ != nullptr;
  }
  iterator lower_bound(const_reference key) {
    iterator i = begin();
    while (i != end() && *i < key) i++;
    return i;
  }
  iterator upper_bound(const_reference key) {
    iterator i = begin();
    while (i != end() && *i <= key) i++;
    return i;
  }
  void erase(iterator pos) {
    node* v = pos.current_;
    if (!v->l && !v->r) {
      if (v->p) {
        if (v == v->p->l) v->p->l = nullptr;
        if (v == v->p->r) v->p->r = nullptr;
      } else
        root_ = nullptr;
    }
    if (v->l && !v->r) {
      v->l->p = v->p;
      if (v->p) {
        if (v == v->p->l) v->p->l = v->l;
        if (v == v->p->r) v->p->r = v->l;
      } else
        root_ = v->l;
    }
    if (v->r && !v->l) {
      v->r->p = v->p;
      if (v->p) {
        if (v == v->p->l) v->p->l = v->r;
        if (v == v->p->r) v->p->r = v->r;
      } else
        root_ = v->r;
    }
    if (v->r && v->l) {
      node* w{nullptr};
      if (!v->p || v == v->p->l) {
        w = iterator(v->r, this).leftiest().current_;
        if (w->p != v) {
          if (w->r) {
            w->p->l = w->r;
            w->r->p = w->p;
          }
          w->p->l = nullptr;
          w->r = v->r;
          v->r->p = w;
        }
        w->l = v->l;
        v->l->p = w;
        w->p = v->p;
        if (v->p)
          v->p->l = w;
        else
          root_ = w;
      } else {
        w = iterator(v->l, this).rightiest().current_;
        if (w->p != v) {
          if (w->l) {
            w->p->r = w->l;
            w->l->p = w->p;
          } else
            w->p->r = nullptr;
          w->l = v->l;
          v->l->p = w;
        }
        w->r = v->r;
        v->r->p = w;
        v->p->r = w;
        w->p = v->p;
      }
    }
    v->~node();
    size_--;
  }
  template <typename... Args>
  std::vector<std::pair<iterator, bool>> insert_many(Args&&... args) {
    std::vector<std::pair<iterator, bool>> result;
    for (auto element : {std::forward<Args>(args)...}) {
      result.push_back(std::pair<iterator, bool>(insert(element), true));
    }
    return result;
  }
};

}  // namespace s21

#endif /* S21_MULTISET */