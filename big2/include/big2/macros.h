//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_BIG2_SCOPED_H_
#define BIG2_STACK_BIG2_SCOPED_H_

#include <gsl/gsl>

/// @private
#define BIG2_UNIQUE_VAR_NAME_JOIN_INNER(prefix, counter) name ## counter
/// @private
#define BIG2_UNIQUE_VAR_NAME_JOIN(prefix, counter) BIG2_UNIQUE_VAR_NAME_JOIN_INNER(prefix, counter)

/**
 * @ingroup(Macros)
 * @brief Generates a unique C identifier for a name.
 */
#define BIG_UNIQUE_VAR_NAME(prefix) BIG2_UNIQUE_VAR_NAME_JOIN(prefix, __COUNTER__)

/**
 * @ingroup(Macros)
 * @brief Creates a scoped assignment to a variable.
 */
#define BIG2_SCOPE(assignment) if constexpr (assignment; true)

/**
 * @ingroup(Macros)
 * @brief Creates a scoped variable by generating an unique name for it.
 */
#define BIG2_SCOPE_VAR(Type) BIG2_SCOPE(Type BIG_UNIQUE_VAR_NAME(_scope_))

#define BIG2_LIKELY_IF(condition) if(GSL_LIKELY(condition)) [[likely]]

#define BIG2_UNLIKELY_IF(condition) if(GSL_UNLIKELY(condition)) [[unlikely]]


#endif //BIG2_STACK_BIG2_SCOPED_H_
