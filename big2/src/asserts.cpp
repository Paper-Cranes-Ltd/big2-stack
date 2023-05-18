//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//
#include <asserts.h>
#include <spdlog/spdlog.h>

namespace big2::detail {

void validate(bool condition, gsl::czstring message, gsl::czstring file, std::uint32_t line) {
  if (!condition) {
    spdlog::error("{0} in '{1}:{2}'", message, file, line);
    throw std::runtime_error(message);
  }
}

bool soft_validate(bool condition, gsl::czstring message, gsl::czstring file, std::uint32_t line) {
  if (!condition) {
    spdlog::error("{0} in '{1}:{2}'", message, file, line);
  }

  return condition;
}

}
