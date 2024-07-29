#ifndef S21_BASIC_LIST
#define S21_BASIC_LIST

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace s21 {

template <typename T>
class BasicList {
 protected:
  class BasicNode {
   public:
    T value_{};
    BasicNode* prev_{nullptr};
    BasicNode* next_{nullptr};

    BasicNode() {}
    BasicNode(T value) : value_(value) {}
    BasicNode(BasicNode* prev, T value) : BasicNode(value) {
      prev_ = prev;
      if (prev) {
        next_ = prev->next_;
        prev->next_ = this;
      }
    }
    BasicNode(T value, BasicNode* next) : BasicNode(value) {
      next_ = next;
      if (next) {
        prev_ = next->prev_;
        next->prev_ = this;
      }
    }
  };

 private:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;
  using node = BasicNode;

 protected:
  size_type size_{};
  node* head_{nullptr};
  node* tail_{nullptr};
  value_type zero_{};

 public:
  // Конструктор пустого контейнера
  BasicList() {}
  // Конструктор контейнера определённого размера
  BasicList(size_type n) {
    while (n--) push_back(value_type());
  }
  // Конструктор из списка инициализации
  BasicList(std::initializer_list<value_type> const& items) {
    for (value_type n : items) push_back(n);
  }
  // Конструктор копирования
  BasicList(const BasicList& other) {
    for (node* i = other.head_; i; i = i->next_) push_back(i->value_);
  }
  // Конструктор перемещения
  BasicList(BasicList&& other) { swap(other); }
  // Деструктор
  ~BasicList() { clear(); }

  BasicList& operator=(BasicList& other) = default;
  BasicList& operator=(BasicList&& other) {
    swap(other);
    other.clear();
    return *this;
  };

 protected:
  bool empty() { return !size_; }
  size_type size() { return size_; }
  const_reference front() {
    if (empty()) return zero_;
    return head_->value_;
  }
  const_reference back() {
    if (empty()) return zero_;
    return tail_->value_;
  }
  void push_front(const_reference value) {
    head_ = new node(value, head_);
    if (empty()) tail_ = head_;
    size_++;
  }
  void push_back(const_reference value) {
    tail_ = new node(tail_, value);
    if (empty()) head_ = tail_;
    size_++;
  }
  void pop_front() {
    if (size_ > 1) {
      head_ = head_->next_;
      delete head_->prev_;
      head_->prev_ = nullptr;
      size_--;
    } else if (size_ == 1) {
      delete head_;
      head_ = nullptr;
      tail_ = nullptr;
      size_--;
    }
  }
  void pop_back() {
    if (size_ > 1) {
      tail_ = tail_->prev_;
      delete tail_->next_;
      tail_->next_ = nullptr;
      size_--;
    } else if (size_ == 1) {
      delete tail_;
      head_ = nullptr;
      tail_ = nullptr;
      size_--;
    }
  }
  void clear() {
    while (size_) pop_back();
  }
  void swap(BasicList& other) {
    if (this != &other) {
      node* tmp_head = head_;
      node* tmp_tail = tail_;
      size_type tmp_size = size_;
      head_ = other.head_;
      tail_ = other.tail_;
      size_ = other.size_;
      other.head_ = tmp_head;
      other.tail_ = tmp_tail;
      other.size_ = tmp_size;
    }
  }
};

}  // namespace s21

#endif  // S21_BASIC_LIST