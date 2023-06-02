//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_VOID_PTR_H_
#define BIG2_STACK_VOID_PTR_H_

#include <cstdint>
#include <type_traits>
#include <concepts>
#include <gsl/gsl>

namespace big2 {

/**
 * @brief A class to ease up the casting between void* and other types. Especially useful with reinterpret casts.
 */
struct VoidPtr final {
  explicit(false) VoidPtr() : pointer(nullptr) {}
  explicit(false) VoidPtr(void *p) : pointer(p) {}
  explicit(false) VoidPtr(auto *p) : pointer(reinterpret_cast<void*>(p)) {}
  template<typename T> requires std::is_pointer_v<T>
  explicit(false) VoidPtr(gsl::not_null<T> p) : pointer(reinterpret_cast<void*>(p.get())) {}
  explicit(false) operator void *() const { return pointer; }

  [[nodiscard]] bool IsValid() const { return pointer != nullptr; }
  [[nodiscard]] void* Get() const { return pointer; }

  auto operator<=>(const VoidPtr other) const { return pointer <=> other.pointer; }

  template<typename T>
  auto operator<=>(const std::decay_t<T> *other) const { return pointer <=> other; }

  /**
   * @brief Reinterpret casts the void pointer to any other type
   */
  template<typename T>
  std::decay_t<T>* RCast() { return reinterpret_cast<std::decay_t<T> *>(pointer); }

  /**
   * @brief Reinterpret casts the void pointer to any other type
   */
  template<typename T>
  const std::decay_t<T>* RCast() const { return reinterpret_cast<const std::decay_t<T> *>(pointer); }

  template<std::integral T>
  VoidPtr operator+(T bytes) const { return static_cast<const std::int8_t*>(pointer) + bytes; }

  template<std::integral T>
  VoidPtr operator-(T bytes) const { return *this + (-bytes); }

  void *pointer;
};

}

#endif //BIG2_STACK_VOID_PTR_H_
