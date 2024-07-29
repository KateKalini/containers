#ifndef S21_QUEUE
#define S21_QUEUE

#include "../part_0/basic_list.h"

namespace s21 {

template <typename T>
class queue : public BasicList<T> {
 private:
  using Parrent = BasicList<T>;

 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;

  using Parrent::BasicList;

  using Parrent::back;
  using Parrent::clear;
  using Parrent::empty;
  using Parrent::front;
  using Parrent::size;
  using Parrent::swap;

  void push(const_reference value) { Parrent::push_back(value); }
  void pop() { Parrent::pop_front(); }
  template <typename... Args>
  void insert_many_back(Args &&...args) {
    ([&] { push(args); }(), ...);
  }
};

}  // namespace s21

#endif /* S21_QUEUE */