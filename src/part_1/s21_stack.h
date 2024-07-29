#ifndef S21_STACK
#define S21_STACK

#include "../part_0/basic_list.h"

namespace s21 {

template <typename T>
class stack : public BasicList<T> {
 private:
  using Parrent = BasicList<T>;

 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;

  using Parrent::BasicList;

  using Parrent::clear;
  using Parrent::empty;
  using Parrent::size;
  using Parrent::swap;

  const_reference top() { return Parrent::back(); }
  void push(const_reference value) { Parrent::push_back(value); }
  void pop() { Parrent::pop_back(); }
  template <typename... Args>
  void insert_many_front(Args &&...args) {
    ([&] { push(args); }(), ...);
  }
};

}  // namespace s21

#endif /* S21_STACK */