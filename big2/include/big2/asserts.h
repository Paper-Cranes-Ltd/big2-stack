//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_BIG2_ASSERTS_H_
#define BIG2_STACK_BIG2_ASSERTS_H_

#include <gsl/gsl>
#include <cstdint>

namespace big2::detail {

void validate(bool condition, gsl::czstring message, gsl::czstring file, std::uint32_t line);
bool soft_validate(bool condition, gsl::czstring message, gsl::czstring file, std::uint32_t line);

}

#define bigValidate(cond, message) big2::detail::validate(cond, message, __FILE__, __LINE__)
#define bigSoftValidate(cond, message) big2::detail::soft_validate(cond, message, __FILE__, __LINE__)


#endif //BIG2_STACK_BIG2_ASSERTS_H_
