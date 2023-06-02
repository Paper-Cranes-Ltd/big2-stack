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

#if __has_include(<format>)
#include <format>
#endif

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

glm::ivec2 GetWindowSize(gsl::not_null<GLFWwindow *> window) {
  glm::ivec2 window_size;
  glfwGetWindowSize(window, &window_size.x, &window_size.y);
  return window_size;
}

VoidPtr GetNativeWindowHandle(gsl::not_null<GLFWwindow *> window) {
#if BX_PLATFORM_LINUX
  return glfwGetX11Window(window);
#elif BX_PLATFORM_OSX
  return glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
  return glfwGetWin32Window(window);
#else
  return nullptr;
#endif
}

void GlfwErrorCallback(std::int32_t error, gsl::czstring description) {
#if __has_include(<format>)
  const std::string message = std::format("[{}] {}", error, description);
#else
  const std::string message(description);
#endif
  big2::Error(message.c_str());
}

glm::ivec2 GetWindowResolution(gsl::not_null<GLFWwindow *> window) {
  glm::ivec2 window_resolution;
  glfwGetFramebufferSize(window, &window_resolution.x, &window_resolution.y);
  return window_resolution;
}

}
