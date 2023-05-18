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
#include <asserts.h>

namespace big2 {

template<typename T>
class IdManager {
  static_assert(std::is_integral_v<T>, "IdManager<T> can only be constructed with numeric types");

 public:
  [[nodiscard]] T Reserve() {
    T potential_id = 0;
    while (potential_id != std::numeric_limits<T>::max()) {
      if (IsFree(potential_id)) {
        Reserve(potential_id);
        return potential_id;
      }
      potential_id++;
    }

    bigValidate(potential_id != std::numeric_limits<T>::max(), "Cannot reserve a new id since all id's are reserved!");
    return std::numeric_limits<T>::max();
  }

  void Reserve(T value) {
    bigSoftValidate(IsFree(value), "Cannot reserve a new id since all id's are reserved!");
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
