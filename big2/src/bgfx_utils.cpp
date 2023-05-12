//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//
#include <big2/bgfx_utils.h>
#include <big2/glfw_utils.h>
#include <GLFW/glfw3.h>
#include <bx/bx.h>
#include <native_window.h>

namespace big2 {

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

}
