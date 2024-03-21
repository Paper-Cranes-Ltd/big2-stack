//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_EXECUTION_H_
#define BIG2_STACK_EXECUTION_H_

#if __has_include(<execution>) && BIG2_EXECUTION_ENABLED
#include <execution>
#if __cpp_lib_execution
#define EXECUTION_POLICY(x) x,
#else
#define EXECUTION_POLICY(x)
#endif
#else
#define EXECUTION_POLICY(x)
#endif

#endif //BIG2_STACK_EXECUTION_H_
