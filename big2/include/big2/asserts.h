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

namespace big2 {

/**
 * @ingroup(Macros)
 * @def bigValidate(cond, message)
 * @brief Will check the condition and if it is false will log it and throw an exception.
 */
#define bigValidate(cond, message) big2::detail::Validate(cond, message, __FILE__, __LINE__)

/**
 * @ingroup(Macros)
 * @def  @def bigSoftValidate(cond, message)
 * @brief Will check the condition and if it is false will log it as an error.
 */
#define bigSoftValidate(cond, message) big2::detail::SoftValidate(cond, message, __FILE__, __LINE__)

/**
 * @ingroup(Macros)
 * @def bigWarning(message)
 * @brief Will log a warning message.
 */
#define bigWarning(message) big2::detail::Warning(message, __FILE__, __LINE__)

/**
 * @ingroup(Macros)
 * @def bigInfo(message)
 * @brief Will log an info message.
 */
#define bigInfo(message) big2::detail::Info(message, __FILE__, __LINE__)

/**
 * @ingroup(Macros)
 * @def bigError(message)
 * @brief Will log an error message.
 */
#define bigError(message) big2::detail::Error(message, __FILE__, __LINE__)

}

#endif //BIG2_STACK_BIG2_ASSERTS_H_
