//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_ASSERTS_H_
#define BIG2_STACK_ASSERTS_H_

#include <gsl/gsl>
#include <cstdint>
#include <string>

#if __has_include(<source_location>)
#include <source_location>
#else

namespace std {
class source_location {
 public:
  [[nodiscard]] static constexpr source_location current() noexcept {
    return {};
  };

  [[nodiscard]] constexpr source_location() noexcept = default;

  [[nodiscard]] constexpr uint_least32_t line() const noexcept {
    return line_;
  }
  [[nodiscard]] constexpr uint_least32_t column() const noexcept {
    return column_;
  }
  [[nodiscard]] constexpr const char *file_name() const noexcept {
    return file_;
  }
  [[nodiscard]] constexpr const char *function_name() const noexcept {
    return function_;
  }

 private:
  uint_least32_t line_{};
  uint_least32_t column_{};
  const char *file_ = "";
  const char *function_ = "";
};
}

#endif

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

inline void Validate(const bool condition, gsl::czstring message, const std::source_location location = std::source_location::current()) {
  big2::detail::Validate(condition, message, location.file_name(), location.function_name(), location.line());
}

inline bool SoftValidate(const bool condition, gsl::czstring message, const std::source_location location = std::source_location::current()) {
  return big2::detail::SoftValidate(condition, message, location.file_name(), location.function_name(), location.line());
}

inline void Error(gsl::czstring message, const std::source_location location = std::source_location::current()) {
  big2::detail::Error(message, location.file_name(), location.function_name(), location.line());
}

inline void Warning(gsl::czstring message, const std::source_location location = std::source_location::current()) {
  big2::detail::Warning(message, location.file_name(), location.function_name(), location.line());
}

inline void Info(gsl::czstring message, const std::source_location location = std::source_location::current()) {
  big2::detail::Info(message, location.file_name(), location.function_name(), location.line());
}

}
#endif //BIG2_STACK_ASSERTS_H_
