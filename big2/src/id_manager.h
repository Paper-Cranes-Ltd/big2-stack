//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_ID_MANAGER_H_
#define BIG2_STACK_ID_MANAGER_H_

#include <set>
#include <type_traits>
#include <functional>
#include <typeinfo>
#include <numeric>
#include <big2/asserts.h>
#include <big2/macros.h>
#include <concepts>
#include <execution>

namespace big2 {

template<std::integral T, T max_value = std::numeric_limits<T>::max()>
class IdManager {
 public:
  [[nodiscard]] T Reserve() {
    for(T current_id = 0; current_id < max_value; current_id++) {
      if(IsFree(current_id)) {
        Reserve(current_id);
        return current_id;
      }
    }

    big2::Validate(false, "Cannot reserve an ID since all IDs are taken");
    return max_value;
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

#endif //BIG2_STACK_ID_MANAGER_H_
