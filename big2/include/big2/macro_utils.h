//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_BIG2_SCOPED_H_
#define BIG2_STACK_BIG2_SCOPED_H_

#include <big2/asserts.h>

/**
 * @ingroup(Macros)
 * @brief Will check the condition and if it is false will log it and throw an exception.
 */
#define bigValidate(cond, message) big2::detail::Validate(cond, message, __FILE__, __LINE__)

/**
 * @ingroup(Macros)
 * @brief Will check the condition and if it is false will log it as an error.
 */
#define bigSoftValidate(cond, message) big2::detail::SoftValidate(cond, message, __FILE__, __LINE__)

/**
 * @ingroup(Macros)
 * @brief Will log a warning message.
 */
#define bigWarning(message) big2::detail::Warning(message, __FILE__, __LINE__)

/**
 * @ingroup(Macros)
 * @brief Will log an info message.
 */
#define bigInfo(message) big2::detail::Info(message, __FILE__, __LINE__)

/**
 * @ingroup(Macros)
 * @brief Will log an error message.
 */
#define bigError(message) big2::detail::Error(message, __FILE__, __LINE__)

/// @private
#define bigUniqueVarNameJoinInner(prefix, counter) name ## counter
/// @private
#define bigUniqueVarNameJoin(prefix, counter) bigUniqueVarNameJoinInner(prefix, counter)

/**
 * @ingroup(Macros)
 * @brief Generates a unique C identifier for a name.
 */
#define bigUniqueVarName(prefix) bigUniqueVarNameJoin(prefix, __COUNTER__)

/**
 * @ingroup(Macros)
 * @brief Creates a scoped assignment to a variable.
 */
#define bigScope(assignment) if constexpr (assignment; true)

/**
 * @ingroup(Macros)
 * @brief Creates a scoped variable by generating an unique name for it.
 */
#define bigScopeVar(Type) bigScope(Type bigUniqueVarName(_scope_))

#endif //BIG2_STACK_BIG2_SCOPED_H_
