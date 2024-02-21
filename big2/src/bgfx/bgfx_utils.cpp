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

void SetNativeWindowData(bgfx::Init &init_obj, gsl::not_null<GLFWwindow *> window) {
#if BX_PLATFORM_LINUX
  init_obj.platformData.ndt = glfwGetX11Display();
#endif
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

bool BgfxSupportsMultipleWindows() {
  const bgfx::Caps* caps = bgfx::getCaps();
  return (caps->supported & BGFX_CAPS_SWAP_CHAIN) != 0;
}

void ResetWindowFrameBuffer(gsl::not_null<GLFWwindow *> window, bgfx::FrameBufferHandle &out_handle) {
  if (bgfx::isValid(out_handle)) {
    bgfx::destroy(out_handle);
  }
  out_handle = CreateWindowFrameBuffer(window);
}

}
