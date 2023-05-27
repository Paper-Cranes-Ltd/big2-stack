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

glm::ivec2 GetWindowSize(gsl::not_null<GLFWwindow *> window) {
  glm::ivec2 window_size;
  glfwGetWindowSize(window, &window_size.x, &window_size.y);
  return window_size;
}

void *GetNativeWindowHandle(gsl::not_null<GLFWwindow *> window) {
#if BX_PLATFORM_LINUX
  return reinterpret_cast<void *>(glfwGetX11Window(window));
#elif BX_PLATFORM_OSX
  return glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
  return glfwGetWin32Window(window);
#else
  return nullptr;
#endif
}

void GlfwErrorCallback(std::int32_t error, gsl::czstring description) {
  big2::Error(std::format("[{}] {}", error, description).c_str());
}

GlfwInitializationScoped::GlfwInitializationScoped() {
  if (is_initialized_) {
    big2::Warning("GLFW is initialized twice");
    return;
  }

  glfwSetErrorCallback(GlfwErrorCallback);

  is_initialized_ = glfwInit() == GLFW_TRUE;
  big2::Validate(is_initialized_, "Couldn't initialize GLFW");
}

GlfwInitializationScoped::~GlfwInitializationScoped() {
  if (!is_initialized_) {
    big2::Warning("GLFW is uninitialized twice");
    return;
  }

  glfwTerminate();
  is_initialized_ = false;
}

GlfwWindowScoped::GlfwWindowScoped(gsl::czstring title, glm::ivec2 size, GLFWmonitor *monitor) {
  constexpr GLFWwindow* shared_window = nullptr;
  window_ = glfwCreateWindow(size.x, size.y, title, monitor, shared_window);
  big2::Validate(window_ != nullptr, "Window couldn't be created!");
}

GlfwWindowScoped::~GlfwWindowScoped() {
  glfwDestroyWindow(window_);
}

GlfwWindowScoped::GlfwWindowScoped(gsl::not_null<GLFWwindow *> window) : window_(window) {

}

}
