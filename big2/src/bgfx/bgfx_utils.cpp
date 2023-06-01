//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//
#include <big2/bgfx/bgfx_utils.h>
#include <big2/glfw/glfw_utils.h>
#include <GLFW/glfw3.h>
#include <bx/bx.h>
#include <native_window.h>
#include <id_manager.h>

namespace big2 {

static IdManager<bgfx::ViewId> view_id_manager;

void SetNativeData(bgfx::Init &init_obj) {
#if BX_PLATFORM_LINUX
  init_obj.platformData.ndt = glfwGetX11Display();
#endif
}

void SetNativeWindowData(bgfx::Init &init_obj, gsl::not_null<GLFWwindow *> window) {
  SetNativeData(init_obj);
  init_obj.platformData.nwh = GetNativeWindowHandle(window);
}

bgfx::FrameBufferHandle CreateWindowFrameBuffer(gsl::not_null<GLFWwindow *> window) {
  glm::ivec2 window_resolution = GetWindowResolution(window);
  return bgfx::createFrameBuffer(GetNativeWindowHandle(window), window_resolution.x, window_resolution.y);
}

bgfx::ViewId ReserveViewId() {
  return view_id_manager.Reserve();
}

std::vector<bgfx::ViewId> GetReservedViewIds() {
  return view_id_manager.GetReservedIds();
}

void FreeViewId(bgfx::ViewId value) {
  view_id_manager.Free(value);
}

void ResetWindowFrameBuffer(gsl::not_null<GLFWwindow *> window, bgfx::FrameBufferHandle &out_handle) {
  if (bgfx::isValid(out_handle)) {
    bgfx::destroy(out_handle);
  }
  out_handle = CreateWindowFrameBuffer(window);
}

BgfxViewScoped::BgfxViewScoped() {
  view_id_ = ReserveViewId();
}

BgfxViewScoped::~BgfxViewScoped() {
  bgfx::resetView(view_id_);
  FreeViewId(view_id_);
}

BgfxViewScoped::BgfxViewScoped(bgfx::ViewId view_id) : view_id_(view_id) {

}
BgfxViewScoped::operator bgfx::ViewId() const {
  return view_id_;
}

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
