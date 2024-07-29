#ifndef S21_LIST
#define S21_LIST

#include <limits>

#include "../part_0/basic_list.h"

namespace s21 {

template <typename T>
class list : public BasicList<T> {
 private:
  using Parrent = BasicList<T>;
  using node = typename Parrent::BasicNode;
  using Parrent::head_;
  using Parrent::size_;
  using Parrent::tail_;

 public:
  class ListIterator {
   public:
    node* current_{nullptr};
    list<T>* owner_{nullptr};

    ListIterator() {}
    ListIterator(node* current, list<T>* owner) {
      current_ = current;
      owner_ = owner;
    }
    ListIterator(const ListIterator& other) = default;
    ListIterator(ListIterator&& other) = default;

    ListIterator& operator=(ListIterator other) {
      current_ = other.current_;
      owner_ = other.owner_;
      return *this;
    }
    bool operator==(ListIterator other) { return current_ == other.current_; }
    bool operator!=(ListIterator other) { return current_ != other.current_; }
    T& operator*() {
      if (!current_) return owner_->tail_->value_;
      return current_->value_;
    }
    ListIterator& operator++() {
      if (!current_)
        current_ = owner_->head_;
      else
        current_ = current_->next_;
      return *this;
    }
    ListIterator operator++(int) {
      ListIterator tmp = *this;
      ++(*this);
      return tmp;
    }
    ListIterator& operator--() {
      if (!current_)
        current_ = owner_->tail_;
      else
        current_ = current_->prev_;
      return *this;
    }
    ListIterator operator--(int) {
      ListIterator tmp = *this;
      --(*this);
      return tmp;
    }
    ListIterator& operator+=(size_t n) {
      while (n--) (*this)++;
      return *this;
    }
    ListIterator operator+(size_t n) {
      ListIterator tmp = *this;
      tmp += n;
      return tmp;
    }
    ListIterator& operator-=(size_t n) {
      while (n--) (*this)--;
      return *this;
    }
    ListIterator operator-(size_t n) {
      ListIterator tmp = *this;
      tmp -= n;
      return tmp;
    }
  };

  class ListConstIterator : public ListIterator {
   public:
    using ListIterator::ListIterator;
    ListConstIterator(const ListIterator& other) : ListIterator(other) {}
    const T& operator*() {
      if (!this->current_) return this->owner_->tail_->value_;
      return this->current_->value;
    }
  };

  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;
  using iterator = ListIterator;
  using const_iterator = ListConstIterator;

  using Parrent::BasicList;

  using Parrent::back;
  using Parrent::clear;
  using Parrent::empty;
  using Parrent::front;
  using Parrent::pop_back;
  using Parrent::pop_front;
  using Parrent::push_back;
  using Parrent::push_front;
  using Parrent::size;
  using Parrent::swap;

  size_type max_size() {
    return (std::numeric_limits<size_type>::max() / sizeof(node) / 2);
  }
  iterator begin() { return iterator(head_, this); }
  iterator end() { return iterator(nullptr, this); }
  iterator insert(iterator pos, const_reference value) {
    iterator result;
    if (pos == begin()) {
      push_front(value);
      result = begin();
    } else if (pos == end()) {
      push_back(value);
      result = end() - 1;
    } else {
      node* insered = new node(value, pos.current_);
      insered->prev_->next_ = insered;
      size_++;
      result = iterator(insered, this);
    }
    return result;
  }
  void erase(iterator pos) {
    if (pos == end()) throw std::logic_error("Bad Position");

    if (pos.current_ == head_)
      pop_front();
    else if (pos.current_ == tail_)
      pop_back();
    else {
      pos.current_->next_->prev_ = pos.current_->prev_;
      pos.current_->prev_->next_ = pos.current_->next_;
      delete pos.current_;
      size_--;
    }
  }
  void reverse() {
    for (iterator i = begin(); i != end(); --i) {
      node* tmp = i.current_->next_;
      i.current_->next_ = i.current_->prev_;
      i.current_->prev_ = tmp;
    }
    node* tmp = head_;
    head_ = tail_;
    tail_ = tmp;
  }
  void unique() {
    if (size_ > 1)
      for (iterator i = begin() + 1; i != end(); ++i)
        if (*i == *(i - 1)) erase(i - 1);
  }
  void sort() {
    for (iterator i = begin(); i != end() - 1; ++i)
      for (iterator j = i + 1; j != end(); j++)
        if (*j < *i) {
          value_type tmp = *i;
          *i = *j;
          *j = tmp;
        }
  }
  void splice(const_iterator pos, list& other) {
    if (this != &other) {
      for (iterator i = other.begin(); i != other.end(); ++i) insert(pos, *i);
      other.clear();
    }
  }
  void merge(list& other) {
    if (this != &other) {
      splice(end(), other);
      sort();
    }
  }

  template <typename... Args>
  iterator insert_many(const_iterator pos, Args&&... args) {
    iterator it(pos);
    for (auto element : {std::forward<Args>(args)...}) {
      insert(it, element);
    }
    return --it;
  }
  template <typename... Args>
  void insert_many_back(Args&&... args) {
    for (auto element : {std::forward<Args>(args)...}) {
      push_back(element);
    }
  }
  template <typename... Args>
  void insert_many_front(Args&&... args) {
    for (auto element : {std::forward<Args>(args)...}) {
      push_front(element);
    }
  }
};

}  // namespace s21

#endif /* S21_LIST */