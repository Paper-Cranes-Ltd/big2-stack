//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_ALGORITHM_H_
#define BIG2_STACK_ALGORITHM_H_

#include <optional>
#include <iterator>

namespace big2 {

template<typename TReturn, std::forward_iterator InputIt, typename TFunc>
[[nodiscard]] std::optional<TReturn> FirstIf(InputIt begin, InputIt end, TFunc &&functor) {
  InputIt result = std::find_if(begin, end, functor);
  if (result != end) {
    return {*result};
  } else {
    return {};
  }
}

template<std::forward_iterator InputIt, typename TFunc>
[[nodiscard]] auto FirstIf(InputIt begin, InputIt end, TFunc &&functor) -> std::optional<typename InputIt::value_type> {
  return FirstIf<typename InputIt::value_type>(begin, end, functor);
}

}

#endif //BIG2_STACK_ALGORITHM_H_
