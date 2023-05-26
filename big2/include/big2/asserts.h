//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_BIG2_ASSERTS_H_
#define BIG2_STACK_BIG2_ASSERTS_H_

#include <gsl/gsl>
#include <cstdint>
#include <string>

namespace big2::detail {

/// @private
void Validate(bool condition, gsl::czstring message, gsl::czstring file, std::uint32_t line);
/// @private
bool SoftValidate(bool condition, gsl::czstring message, gsl::czstring file, std::uint32_t line);
/// @private
void Error(gsl::czstring message, gsl::czstring file, std::uint32_t line);
/// @private
void Warning(gsl::czstring message, gsl::czstring file, std::uint32_t line);
/// @private
void Info(gsl::czstring message, gsl::czstring file, std::uint32_t line);

}
#endif //BIG2_STACK_BIG2_ASSERTS_H_
