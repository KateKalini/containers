#ifndef S21_BASIC_TREE
#define S21_BASIC_TREE

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace s21 {

template <typename T>
class BasicTree {
 protected:
  class TreeNode {
   public:
    T data_;
    TreeNode* p{nullptr};
    TreeNode* l{nullptr};
    TreeNode* r{nullptr};

    TreeNode() {}
    TreeNode(const T value) : data_(value) {}
    TreeNode(const T value, TreeNode* parrent) : data_(value) { p = parrent; }
    ~TreeNode() { data_.~T(); }
  };

  class TreeIterator {
   public:
    TreeNode* current_{nullptr};
    BasicTree* owner_{nullptr};
    TreeIterator() {}
    TreeIterator(TreeNode* current, BasicTree* owner) {
      current_ = current;
      owner_ = owner;
    }
    TreeIterator(const TreeIterator& other) = default;
    TreeIterator(TreeIterator&& other) = default;

    TreeIterator& operator=(TreeIterator other) {
      current_ = other.current_;
      owner_ = other.owner_;
      return *this;
    }
    TreeIterator leftiest() {
      if (!current_) current_ = owner_->root_;
      while (current_ && current_->l) current_ = current_->l;
      return *this;
    }
    TreeIterator rightiest() {
      if (!current_) current_ = owner_->root_;
      while (current_ && current_->r) current_ = current_->r;
      return *this;
    }
    TreeIterator& operator++() {
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
    TreeIterator operator++(int) {
      TreeIterator tmp = *this;
      ++(*this);
      return tmp;
    }
    TreeIterator& operator--() {
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
    TreeIterator operator--(int) {
      TreeIterator tmp = *this;
      --(*this);
      return tmp;
    }
    T& operator*() {
      if (!current_) return owner_->zero_;
      return current_->data_;
    }
    bool operator==(TreeIterator other) { return current_ == other.current_; }
    bool operator!=(TreeIterator other) { return current_ != other.current_; }
  };

  class TreeConstIterator : public TreeIterator {
    using TreeIterator::TreeIterator;
    TreeConstIterator(const TreeIterator& other) : TreeIterator(other) {}
    const T& operator*() {
      if (!this->current_) return this->owner_->zero_;
      return this->current_->data_;
    }
  };

 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;
  using node = TreeNode;
  using iterator = TreeIterator;
  value_type zero_{};

  BasicTree() {}
  BasicTree(std::initializer_list<value_type> const& items) {
    for (auto n : items) this->insert(n);
  }
  BasicTree(BasicTree& other) {
    for (auto i = other.begin(); i != other.end(); ++i) this->insert(*i);
  }
  BasicTree(BasicTree&& other) {
    root_ = other.root_;
    size_ = other.size_;
    other.root_ = nullptr;
    other.size_ = 0;
  }
  ~BasicTree() {
    clear();
    zero_.~value_type();
  }
  BasicTree& operator=(BasicTree&& other) {
    root_ = other.root_;
    size_ = other.size_;
    other.root_ = nullptr;
    other.size_ = 0;
    return *this;
  }

 protected:
  node* root_{nullptr};
  size_type size_{};

  bool empty() { return !size_; }
  size_type size() { return size_; }
  void swap(BasicTree& other) {
    if (this != &other) {
      node* tmp_root = root_;
      size_type tmp_size = size_;
      root_ = other.root_;
      size_ = other.size_;
      other.root_ = tmp_root;
      other.size_ = tmp_size;
    }
  }
  void merge(BasicTree& other) {
    for (auto i = other.begin(); i != other.end(); ++i) insert(*i);
    // other.clear();
  }
  size_type max_size() { return (size_t(-1) / sizeof(BasicTree)); }
  iterator begin() { return iterator(root_, this).leftiest(); }
  iterator end() { return iterator(nullptr, this); }
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
  void clear() {
    while (size_) erase(begin());
  }
  std::pair<iterator, bool> insert(const_reference value) {
    node** pos = pos_search(value, &root_);
    bool is_inserted = false;
    if (size_ == 0) {
      *pos = new node(value);
      size_++;
      is_inserted = true;
    } else if ((*pos)->data_ != value) {
      if (value < (*pos)->data_) {
        (*pos)->l = new node(value, *pos);
        pos = &(*pos)->l;
      }
      if (value > (*pos)->data_) {
        (*pos)->r = new node(value, *pos);
        pos = &(*pos)->r;
      }
      size_++;
      is_inserted = true;
    }
    return std::pair<iterator, bool>(iterator(*pos, this), is_inserted);
  }
  iterator find(const_reference value) {
    iterator result;
    node** pos = pos_search(value, &root_);
    if ((*pos)->data_ == value) result = iterator(*pos, this);
    return result;
  }
  bool contains(const_reference value) {
    iterator pos = find(value);
    return pos.current_ != nullptr;
  }
  node** pos_search(const_reference v, node** n) {
    node** pos{nullptr};
    if (*n == nullptr) {
      pos = n;
    } else if ((*n)->data_ == v) {
      pos = n;
    } else if (v < (*n)->data_) {
      if ((*n)->l == nullptr)
        pos = n;
      else
        pos = pos_search(v, &((*n)->l));
    } else if (v > (*n)->data_) {
      if ((*n)->r == nullptr)
        pos = n;
      else
        pos = pos_search(v, &((*n)->r));
    }
    return pos;
  }
};

}  // namespace s21

#endif  // S21_BASIC_TREE