//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//
#include <big2/bgfx/bgfx_callback_handler.h>

void big2::BgfxCallbackHandler::fatal(const char *file_path, uint16_t line, bgfx::Fatal::Enum code, const char *message) {
  if (bgfx::Fatal::DebugCheck == code) {
    big2::Info(std::format("BGFX Error '{}:{}': {}", file_path, line, message).c_str());
  } else {
    big2::Error(std::format("BGFX Error '{}:{}': {}", file_path, line, message).c_str());
  }
}

void big2::BgfxCallbackHandler::traceVargs(const char *_filePath, uint16_t _line, const char *_format, va_list _argList) {

}

void big2::BgfxCallbackHandler::profilerBegin(const char *, uint32_t, const char *, uint16_t) {
}

void big2::BgfxCallbackHandler::profilerBeginLiteral(const char *, uint32_t, const char *, uint16_t) {
}

void big2::BgfxCallbackHandler::profilerEnd() {
}

uint32_t big2::BgfxCallbackHandler::cacheReadSize(uint64_t) {
  return 0;
}

bool big2::BgfxCallbackHandler::cacheRead(uint64_t, void *, uint32_t) {
  return false;
}

void big2::BgfxCallbackHandler::cacheWrite(uint64_t, const void *, uint32_t) {
}

void big2::BgfxCallbackHandler::screenShot(const char *_filePath, uint32_t _width, uint32_t _height, uint32_t _pitch, const void *_data, uint32_t _size, bool _yflip) {
  big2::Warning("Screenshot not implemented.");
}

void big2::BgfxCallbackHandler::captureBegin(uint32_t, uint32_t, uint32_t, bgfx::TextureFormat::Enum, bool) {
  big2::Warning("Using capture without callback (a.k.a. pointless).");
}

void big2::BgfxCallbackHandler::captureEnd() {
}

void big2::BgfxCallbackHandler::captureFrame(const void *, uint32_t) {
}
