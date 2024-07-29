#ifndef S21_MAP
#define S21_MAP

#include "../part_0/basic_tree.h"
#include <vector>

namespace s21 {

template <typename Key, typename T>
class map : public BasicTree<std::pair<const Key, T>> {
 private:
  using Parrent = BasicTree<std::pair<const Key, T>>;
  using Parrent::root_;
  using Parrent::size_;
  using Parrent::zero_;
  using typename Parrent::TreeConstIterator;
  using typename Parrent::TreeIterator;
  using typename Parrent::TreeNode;

 public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const Key, T>;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = TreeIterator;
  using const_iterator = TreeConstIterator;
  using size_type = size_t;
  using node = TreeNode;

  using Parrent::BasicTree;

  using Parrent::begin;
  using Parrent::clear;
  using Parrent::empty;
  using Parrent::end;
  using Parrent::erase;
  using Parrent::max_size;
  using Parrent::size;
  using Parrent::swap;

  mapped_type& operator[](const key_type key) {
    if (!contains(key))
      return insert(key, mapped_type()).first.current_->data_.second;
    else
      return at(key);
  }
  mapped_type& at(const Key& key) {
    if (!contains(key)) throw std::out_of_range("Bad Key");
    node** pos = pos_search(key, &root_);
    return (*pos)->data_.second;
  }
  std::pair<iterator, bool> insert(const value_type& value) {
    return insert(value.first, value.second);
  }
  std::pair<iterator, bool> insert_or_assign(const Key& key, const T& obj) {
    std::pair<iterator, bool> pos = insert(key, obj);
    if (pos.second == false) {
      pos.first.current_->data_.second = obj;
      pos.second = true;
    }
    return pos;
  }
  std::pair<iterator, bool> insert(const Key& key, const T& obj) {
    node** pos = pos_search(key, &root_);
    bool is_inserted = false;
    if (size_ == 0) {
      *pos = new node(value_type(key, obj));
      size_++;
      is_inserted = true;
    } else if ((*pos)->data_.first != key) {
      if (key < (*pos)->data_.first) {
        (*pos)->l = new node(value_type(key, obj), *pos);
        pos = &(*pos)->l;
      }
      if (key > (*pos)->data_.first) {
        (*pos)->r = new node(value_type(key, obj), *pos);
        pos = &(*pos)->r;
      }
      size_++;
      is_inserted = true;
    }
    return std::pair<iterator, bool>(iterator(*pos, this), is_inserted);
  }
  bool contains(const Key& key) {
    node** pos = pos_search(key, &root_);
    bool result = false;
    if ((*pos)->data_.first == key) result = true;
    return result;
  }
  void merge(map& other) {
    for (auto i = other.begin(); i != other.end(); ++i) insert(*i);
  }
  template <typename... Args>
  std::vector<std::pair<iterator, bool>> insert_many(Args&&... args) {
    std::vector<std::pair<iterator, bool>> result;
    for (auto element : {std::forward<Args>(args)...}) {
      result.push_back(insert(element));
    }
    return result;
  }

 private:
  node** pos_search(const key_type& v, node** n) {
    node** pos{nullptr};
    if (*n == nullptr) {
      pos = n;
    } else if ((*n)->data_.first == v) {
      pos = n;
    } else if (v < (*n)->data_.first) {
      if ((*n)->l == nullptr)
        pos = n;
      else
        pos = pos_search(v, &((*n)->l));
    } else if (v > (*n)->data_.first) {
      if ((*n)->r == nullptr)
        pos = n;
      else
        pos = pos_search(v, &((*n)->r));
    }
    return pos;
  }
};

}  // namespace s21

#endif /* S21_MAP */