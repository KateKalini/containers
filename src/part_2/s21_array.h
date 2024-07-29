#ifndef S21_ARRAY
#define S21_ARRAY

#include "../part_0/basic_array.h"

namespace s21 {

template <typename T, std::size_t N>
class array : public BasicArray<T, N> {
 private:
  using Parrent = BasicArray<T, N>;
  using Parrent::data_;
  using Parrent::size_;

 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;
  using iterator = T*;

  array() {
    size_ = N;
    data_ = new value_type[N]{};
  }
  array(std::initializer_list<value_type> const& items) : array() {
    size_type i{};
    for (auto n : items) data_[i++] = n;
  }
  array(const array& other) : array() {
    for (size_type i = 0; i < size_; i++) data_[i] = other.data_[i];
  }
  array(array&& other) {
    size_ = other.size_;
    data_ = other.data_;
    other.size_ = 0;
    other.data_ = nullptr;
  }
  ~array() { delete[] data_; }
  array& operator=(array&& other) {
    if (this != &other) {
      size_ = other.size_;
      data_ = other.data_;
      other.size_ = 0;
      other.data_ = nullptr;
    }
    return *this;
  }
  size_type max_size() { return size_; }
  void fill(const_reference value) {
    for (size_type i = 0; i < size_; i++) data_[i] = value;
  }
};

}  // namespace s21

#endif /* S21_ARRAY */