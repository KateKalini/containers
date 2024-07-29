#ifndef S21_BASIC_ARRAY
#define S21_BASIC_ARRAY

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace s21 {

template <typename T, std::size_t N>
class BasicArray {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;
  using iterator = T *;
  using const_iterator = const T *;

 protected:
  size_type size_{};
  value_type *data_{nullptr};

 public:
  // Конструктор пустого массива
  BasicArray(){};

  iterator data() { return data_; }
  iterator begin() { return data_; }
  iterator end() { return data_ + size_; }
  reference at(size_type pos) {
    if (pos >= size_) throw std::out_of_range("Index out of range");
    return data_[pos];
  }
  reference operator[](size_type pos) { return at(pos); }
  const_reference front() { return at(0); }
  const_reference back() { return at(size_ - 1); }
  bool empty() { return !size_; }
  size_type size() { return size_; }
  void swap(BasicArray &other) {
    if (this != &other) {
      size_type tmp_size = size_;
      value_type *tmp_data = data_;
      size_ = other.size_;
      data_ = other.data_;
      other.size_ = tmp_size;
      other.data_ = tmp_data;
    }
  }
};

}  // namespace s21

#endif  // S21_BASIC_ARRAY