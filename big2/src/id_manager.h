//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_BIG2_ID_MANAGER_H_
#define BIG2_STACK_BIG2_ID_MANAGER_H_

#include <set>
#include <type_traits>
#include <range/v3/all.hpp>
#include <gsl/gsl>
#include <functional>
#include <typeinfo>

namespace big2 {

template<typename T>
class IdManager {
  static_assert(std::is_integral_v<T>, "IdManager<T> can only be constructed with numeric types");

 public:
  [[nodiscard]] T Reserve() {
    std::function<bool(T value)> filter_predicate = std::bind(&IdManager<T>::IsFree, this, std::placeholders::_1);
    auto generator = ranges::views::ints(0, ranges::unreachable) | ranges::views::filter(filter_predicate) | ranges::views::take(1);
    const T first_free = generator.front();
    Ensures(IsFree(first_free));
    ids_.insert(first_free);
    return first_free;
  }

  void Reserve(T value) {
    Expects(IsFree(value));
    ids_.insert(value);
  }

  void Free(T value) {
    ids_.erase(value);
  }

  [[nodiscard]] bool IsFree(T value) const {
    return ids_.find(value) == ids_.end();
  }

  [[nodiscard]] bool IsReserved(T value) const {
    return !IsFree(value);
  }

  [[nodiscard]] std::vector<T> GetReservedIds() const {
    return std::vector<T>(ids_.begin(), ids_.end());
  }

 private:
  std::set<T> ids_;
};

}

#endif //BIG2_STACK_BIG2_ID_MANAGER_H_
