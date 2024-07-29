#ifndef S21_VECTOR
#define S21_VECTOR

#include <limits>

#include "../part_0/basic_array.h"

namespace s21 {

template <typename T>
class vector : public BasicArray<T, 0> {
 private:
  using Parrent = BasicArray<T, 0>;
  using Parrent::data_;
  using Parrent::size_;
  size_t capacity_{};

 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;
  using iterator = T*;
  using const_iterator = const T*;

  using Parrent::begin;
  using Parrent::end;

  vector() {}
  vector(size_type n) {
    data_ = new value_type[n]{};
    capacity_ = n;
    size_ = n;
  }
  vector(std::initializer_list<value_type> const& items) {
    data_ = new value_type[items.size()]{};
    capacity_ = items.size();
    for (auto n : items) data_[size_++] = n;
  }
  vector(const vector& other) {
    data_ = new value_type[other.size_]{};
    for (size_type i = 0; i < other.size_; i++) data_[i] = other.data_[i];
    capacity_ = other.size_;
    size_ = other.size_;
  }
  vector(vector&& other) {
    capacity_ = other.capacity_;
    size_ = other.size_;
    data_ = other.data_;
    other.capacity_ = 0;
    other.size_ = 0;
    other.data_ = nullptr;
  }
  ~vector() { delete[] data_; }
  vector& operator=(vector&& other) {
    if (this != &other) {
      capacity_ = other.capacity_;
      size_ = other.size_;
      data_ = other.data_;
      other.capacity_ = 0;
      other.size_ = 0;
      other.data_ = nullptr;
    }
    return *this;
  }
  size_type capacity() { return capacity_; }
  size_type max_size() { return std::numeric_limits<std::ptrdiff_t>::max(); }
  void reserve(size_type size) {
    if (size > capacity_) {
      value_type* new_data = new value_type[size]{};
      capacity_ = size;
      for (size_type i = 0; i < size_; i++) new_data[i] = data_[i];
      delete[] data_;
      data_ = new_data;
    }
  }
  void shrink_to_fit() {
    if (capacity_ > size_) {
      value_type* new_data = new value_type[size_]{};
      capacity_ = size_;
      for (size_type i = 0; i < size_; i++) new_data[i] = data_[i];
      delete[] data_;
      data_ = new_data;
    }
  }
  void swap(vector& other) {
    Parrent::swap(other);
    size_type tmp_capacity = capacity_;
    capacity_ = other.capacity_;
    other.capacity_ = tmp_capacity;
  }
  void clear() {
    for (size_type i = 0; i < size_; i++) data_[i].~value_type();
    size_ = 0;
  }
  void push_back(const_reference value) {
    if (!capacity_) reserve(1);
    if (size_ == capacity_) reserve(capacity_ * 2);
    data_[size_] = value;
    size_++;
  }
  void pop_back() {
    data_[size_ - 1].~value_type();
    size_--;
  }
  iterator insert(iterator pos, const_reference value) {
    size_type index = pos - data_;
    if (index > size_) throw std::out_of_range("Position out of range");
    if (!capacity_) reserve(1);
    if (size_ >= capacity_) reserve(capacity_ * 2);
    for (size_type i = size_; i > index; i--) data_[i] = data_[i - 1];
    data_[index] = value;
    size_++;
    return data_ + index;
  }
  void erase(iterator pos) {
    size_type index = pos - data_;
    data_[index].~value_type();
    for (size_type i = index; i < size_ - 1; i++) data_[i] = data_[i + 1];
    size_--;
  }

  template <typename... Args>
  iterator insert_many(const_iterator pos, Args&&... args) {
    iterator old_begin = begin();
    if (capacity_ < size_ + sizeof...(args)) reserve(size_ + sizeof...(args));
    iterator new_pos = begin() + (pos - old_begin);
    iterator new_pos_copy = new_pos;
    iterator last_element = std::prev(end());
    size_ += sizeof...(args);
    for (iterator i = std::prev(end()); i >= new_pos + sizeof...(args); i--) {
      *i = *std::move(last_element);
      last_element--;
    }
    vector<T> arg_values = {args...};
    for (auto element : arg_values) {
      *new_pos = std::move(element);
      new_pos++;
    }
    return new_pos_copy;
  };

  template <typename... Args>
  void insert_many_back(Args&&... args) {
    if (capacity_ < size_ + sizeof...(args)) reserve(size_ + sizeof...(args));
    iterator new_pos = end();
    size_ += sizeof...(args);
    vector<T> arg_values = {args...};
    for (auto element : arg_values) {
      *new_pos = std::move(element);
      new_pos++;
    }
  };
};

}  // namespace s21

#endif /* S21_VECTOR */
