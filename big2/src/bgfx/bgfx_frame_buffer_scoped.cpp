//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//
#include <big2/bgfx/bgfx_frame_buffer_scoped.h>
#include <big2/bgfx/bgfx_utils.h>

namespace big2 {

BgfxFrameBufferScoped::BgfxFrameBufferScoped(gsl::not_null<GLFWwindow *> window) {
  handle_ = big2::CreateWindowFrameBuffer(window);
}

BgfxFrameBufferScoped::BgfxFrameBufferScoped(bgfx::FrameBufferHandle handle) : handle_(handle) {

}

BgfxFrameBufferScoped::~BgfxFrameBufferScoped() {
  bgfx::destroy(handle_);
}

BgfxFrameBufferScoped::operator bgfx::FrameBufferHandle() const {
  return handle_;
}

BgfxFrameBufferScoped::operator bgfx::FrameBufferHandle &() {
  return handle_;
}

}
