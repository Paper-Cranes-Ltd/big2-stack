//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_BIG2_VARIANTS_H_
#define BIG2_STACK_BIG2_VARIANTS_H_

#include <variant>
#include <cstdint>

#define bigDefineVariantEnum(TypeName, ...) \
  using TypeName = std::variant<__VA_ARGS__>; \
  enum class TypeName##Enum : std::size_t { __VA_ARGS__ }

#endif //BIG2_STACK_BIG2_VARIANTS_H_
