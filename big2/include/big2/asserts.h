//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_BIG2_ASSERTS_H_
#define BIG2_STACK_BIG2_ASSERTS_H_

#include <gsl/gsl>
#include <cstdint>
#include <string>
#include <source_location>

namespace big2 {

#pragma region Detail Implementation
namespace detail {

/// @private
void Validate(bool condition, gsl::czstring message, gsl::czstring file, gsl::czstring function, std::uint32_t line);
/// @private
bool SoftValidate(bool condition, gsl::czstring message, gsl::czstring file, gsl::czstring function, std::uint32_t line);
/// @private
void Error(gsl::czstring message, gsl::czstring file, gsl::czstring function, std::uint32_t line);
/// @private
void Warning(gsl::czstring message, gsl::czstring file, gsl::czstring function, std::uint32_t line);
/// @private
void Info(gsl::czstring message, gsl::czstring file, gsl::czstring function, std::uint32_t line);

}
#pragma endregion


inline void Validate(const bool condition, gsl::czstring message, const std::source_location location = std::source_location::current())
{
  big2::detail::Validate(condition, message, location.file_name(), location.function_name(), location.line());
}

inline bool SoftValidate(const bool condition, gsl::czstring message, const std::source_location location = std::source_location::current())
{
  return big2::detail::SoftValidate(condition, message, location.file_name(), location.function_name(), location.line());
}

inline void Error(gsl::czstring message, const std::source_location location = std::source_location::current())
{
  big2::detail::Error( message, location.file_name(), location.function_name(), location.line());
}

inline void Warning(gsl::czstring message, const std::source_location location = std::source_location::current())
{
  big2::detail::Warning( message, location.file_name(), location.function_name(), location.line());
}

inline void Info(gsl::czstring message, const std::source_location location = std::source_location::current())
{
  big2::detail::Info( message, location.file_name(), location.function_name(), location.line());
}

}
#endif //BIG2_STACK_BIG2_ASSERTS_H_
