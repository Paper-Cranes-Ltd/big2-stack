//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#include <big2/glfw/glfw_utils.h>
#include <GLFW/glfw3.h>
#include <bx/bx.h>
#include <native_window.h>
#include <spdlog/spdlog.h>
#include <big2/macros.h>
#include <big2/asserts.h>

namespace big2 {
gsl::span<GLFWmonitor *> GetMonitors() {
  std::int32_t count;
  GLFWmonitor **monitors = glfwGetMonitors(&count);
  return {monitors, static_cast<std::size_t>(count)};
}

glm::ivec2 GetMonitorResolution(gsl::not_null<GLFWmonitor *> monitor) {
  const GLFWvidmode *mode = glfwGetVideoMode(monitor);
  return {mode->width, mode->height};
}

glm::ivec2 GetMonitorPosition(gsl::not_null<GLFWmonitor *> monitor) {
  glm::ivec2 position;
  glfwGetMonitorPos(monitor, &position.x, &position.y);
  return position;
}

std::int32_t GetMonitorRefreshRate(gsl::not_null<GLFWmonitor *> monitor) {
  gsl::not_null<const GLFWvidmode *> mode = glfwGetVideoMode(monitor);
  return mode->refreshRate;
}

glm::u16vec2 GetWindowSize(gsl::not_null<GLFWwindow *> window) {
  glm::ivec2 window_size;
  glfwGetWindowSize(window, &window_size.x, &window_size.y);
  return {static_cast<std::uint16_t>(window_size.x), static_cast<std::uint16_t>(window_size.y)};
}

VoidPtr GetNativeWindowHandle(gsl::not_null<GLFWwindow *> window) {
#if BX_PLATFORM_LINUX
  return VoidPtr(glfwGetX11Window(window));
#elif BX_PLATFORM_OSX
  return glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
  return glfwGetWin32Window(window);
#else
  return nullptr;
#endif
}

void GlfwErrorCallback(std::int32_t, gsl::czstring description) {
  big2::Error(description);
}

glm::u16vec2 GetWindowResolution(gsl::not_null<GLFWwindow *> window) {
  glm::ivec2 window_resolution;
  glfwGetFramebufferSize(window, &window_resolution.x, &window_resolution.y);
  return {static_cast<std::uint16_t>(window_resolution.x), static_cast<std::uint16_t>(window_resolution.y)};
}
}
