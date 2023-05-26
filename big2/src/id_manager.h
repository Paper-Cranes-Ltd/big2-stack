//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_BIG2_ID_MANAGER_H_
#define BIG2_STACK_BIG2_ID_MANAGER_H_

#include <set>
#include <type_traits>
#include <functional>
#include <typeinfo>
#include <numeric>
#include <big2/asserts.h>
#include <big2/macros.h>
#include <ranges>
#include <concepts>
#include <execution>

namespace big2 {

template<std::integral T, T max_value = std::numeric_limits<T>::max()>
class IdManager {
 public:
  [[nodiscard]] T Reserve() {
    auto is_free = std::bind(&IdManager::IsFree, this, std::placeholders::_1);
    std::ranges::view auto free_ids = std::views::iota(static_cast<T>(0), max_value) | std::views::filter(is_free) | std::views::take(1);
    big2::Validate(!free_ids.empty(), "Cannot reserve and ID since all IDs are taken");
    T id = free_ids.front();
    Reserve(id);
    return id;
  }

  void Reserve(T value) {
    big2::SoftValidate(IsFree(value), "Cannot reserve a new id since all id's are reserved!");
    ids_.insert(value);
  }

  void Free(T value) {
    ids_.erase(value);
  }

  [[nodiscard]] bool IsFree(T value) const {
    return !ids_.contains(value);
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
