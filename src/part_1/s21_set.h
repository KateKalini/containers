#ifndef S21_SET
#define S21_SET

#include "../part_0/basic_tree.h"
#include <vector>

namespace s21 {

template <typename T>
class set : public BasicTree<T> {
 private:
  using Parrent = BasicTree<T>;
  using typename Parrent::TreeConstIterator;
  using typename Parrent::TreeIterator;

 public:
  using key_type = T;
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = TreeIterator;
  using const_iterator = TreeConstIterator;
  using size_type = size_t;

  using Parrent::BasicTree;

  using Parrent::begin;
  using Parrent::clear;
  using Parrent::contains;
  using Parrent::empty;
  using Parrent::end;
  using Parrent::erase;
  using Parrent::find;
  using Parrent::insert;
  using Parrent::max_size;
  using Parrent::merge;
  using Parrent::size;
  using Parrent::swap;

  template <typename... Args>
  std::vector<std::pair<iterator, bool>> insert_many(Args &&...args) {
    std::vector<std::pair<iterator, bool>> result;
    for (auto element : {std::forward<Args>(args)...}) {
      result.push_back(insert(element));
    }
    return result;
  }
};

}  // namespace s21

#endif /* S21_SET */
