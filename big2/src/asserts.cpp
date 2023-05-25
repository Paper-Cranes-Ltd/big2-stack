//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//
#include <big2/asserts.h>
#include <spdlog/spdlog.h>

namespace big2::detail {

void Validate(bool condition, gsl::czstring message, gsl::czstring file, std::uint32_t line) {
  if (!condition) {
    spdlog::error("'{0}' at '{1}:{2}'", message, file, line);
    throw std::runtime_error(message);
  }
}

bool SoftValidate(bool condition, gsl::czstring message, gsl::czstring file, std::uint32_t line) {
  if (!condition) {
    spdlog::error("'{0}' at '{1}:{2}'", message, file, line);
  }

  return condition;
}

void Info(gsl::czstring message, gsl::czstring file, std::uint32_t line) {
  spdlog::info("'{0}' at '{1}:{2}'", message, file, line);
}

void Warning(gsl::czstring message, gsl::czstring file, std::uint32_t line) {
  spdlog::warn("'{0}' at '{1}:{2}'", message, file, line);
}

void Error(gsl::czstring message, gsl::czstring file, std::uint32_t line) {
  spdlog::error("'{0}' at '{1}:{2}'", message, file, line);
}

}
