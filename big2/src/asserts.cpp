//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//
#include <big2/asserts.h>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <big2/macros.h>

namespace big2::detail {
void Validate(bool condition, gsl::czstring message, gsl::czstring file, gsl::czstring function, std::uint32_t line) {
  BIG2_UNLIKELY_IF(!condition) {
    spdlog::error("{3}: '{0}' at '{1}:{2}'", message, file, line, function);
    throw std::runtime_error(message);
  }
}

bool SoftValidate(bool condition, gsl::czstring message, gsl::czstring file, gsl::czstring function, std::uint32_t line) {
  BIG2_UNLIKELY_IF(!condition) {
    spdlog::error("{3}: '{0}' at '{1}:{2}'", message, file, line, function);
  }

  return condition;
}

void Info(gsl::czstring message, gsl::czstring file, gsl::czstring function, std::uint32_t line) {
  spdlog::info("{3}: '{0}' at '{1}:{2}'", message, file, line, function);
}

void Warning(gsl::czstring message, gsl::czstring file, gsl::czstring function, std::uint32_t line) {
  spdlog::warn("{3}: '{0}' at '{1}:{2}'", message, file, line, function);
}

void Error(gsl::czstring message, gsl::czstring file, gsl::czstring function, std::uint32_t line) {
  spdlog::error("{3}: '{0}' at '{1}:{2}'", message, file, line, function);
}

}
