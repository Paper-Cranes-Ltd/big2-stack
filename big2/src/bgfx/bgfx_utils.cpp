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

bgfx::FrameBufferHandle CreateWindowFramebuffer(gsl::not_null<GLFWwindow *> window) {
  glm::ivec2 window_size = GetWindowSize(window);
  return bgfx::createFrameBuffer(GetNativeWindowHandle(window), window_size.x, window_size.y);
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

void UpdateFrameBuffer(gsl::not_null<GLFWwindow *> window, bgfx::FrameBufferHandle &outHandle) {
  if (bgfx::isValid(outHandle)) {
    bgfx::destroy(outHandle);
  }
  outHandle = CreateWindowFramebuffer(window);
}

}
