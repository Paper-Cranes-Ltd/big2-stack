//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//
#include <big2/window.h>
#include <big2/asserts.h>
#include <GLFW/glfw3.h>
#include <big2/bgfx/bgfx_utils.h>
#include <big2/glfw/glfw_utils.h>

namespace big2 {

Window::Window(gsl::czstring title, glm::ivec2 size, GLFWmonitor *monitor) {
  constexpr GLFWwindow *shared_window = nullptr;
  window_ = glfwCreateWindow(size.x, size.y, title, monitor, shared_window);
  big2::Validate(window_ != nullptr, "Window couldn't be created!");
  Initialize();
}

Window::~Window() {
  if (is_scoped_) {
    Dispose();
  }
}

Window::Window(gsl::not_null<GLFWwindow *> window) : window_(window) {
  Initialize();
}

Window &Window::SetIsScoped(bool scoped) {
  is_scoped_ = scoped;
  return *this;
}

void Window::Dispose() {
  glfwDestroyWindow(window_);
  bgfx::resetView(view_id_);
  bgfx::destroy(frame_buffer_);
  FreeViewId(view_id_);

  view_id_ = BGFX_INVALID_HANDLE;
  frame_buffer_ = BGFX_INVALID_HANDLE;
}

void Window::Initialize() {
  const glm::ivec2 initial_window_resolution = GetResolution();

  bgfx::createFrameBuffer(GetNativeWindowHandle(window_), initial_window_resolution.x, initial_window_resolution.y, bgfx::TextureFormat::RGBA8, bgfx::TextureFormat::D24S8);
  frame_buffer_ = bgfx::createFrameBuffer(GetNativeWindowHandle(window_), initial_window_resolution.x, initial_window_resolution.y);
  view_id_ = ReserveViewId();
  bgfx::setViewFrameBuffer(view_id_, frame_buffer_);
  bgfx::setViewRect(view_id_, 0, 0, initial_window_resolution.x, initial_window_resolution.y);
  bgfx::setViewClear(view_id_, BGFX_CLEAR_COLOR, 0x000000FF);
}

void Window::SetClearColor(std::uint32_t rgba) const {
  bgfx::setViewClear(view_id_, BGFX_CLEAR_COLOR, rgba);
}

glm::ivec2 Window::GetSize() const {
  return big2::GetWindowSize(window_);
}

glm::ivec2 Window::GetResolution() const {
  return big2::GetWindowResolution(window_);
}

void Window::SetFrameSize(glm::ivec2 size) {
  bgfx::destroy(frame_buffer_);
  frame_buffer_ = bgfx::createFrameBuffer(GetNativeWindowHandle(window_), size.x, size.y);
  bgfx::setViewFrameBuffer(view_id_, frame_buffer_);
  bgfx::setViewRect(view_id_, 0, 0, size.x, size.y);
}
bool Window::GetShouldClose() const {
  return glfwWindowShouldClose(window_);
}

}
