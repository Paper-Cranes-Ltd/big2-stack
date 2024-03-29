//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//
#include <big2/window.h>
#include <big2/asserts.h>
#include <GLFW/glfw3.h>
#include <big2/bgfx/bgfx_utils.h>
#include <big2/glfw/glfw_utils.h>
#include <big2/event_queue.h>

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

Window::Window(gsl::not_null<GLFWwindow *> window)
  : window_(window) {
  Initialize();
}

Window &Window::SetIsScoped(bool scoped) {
  is_scoped_ = scoped;
  return *this;
}

bool Window::GetIsResizable() const {
  return glfwGetWindowAttrib(window_, GLFW_RESIZABLE);
}

Window & Window::SetIsResizable(bool is_resizable) {
  glfwSetWindowAttrib(window_, GLFW_RESIZABLE, is_resizable);
  return *this;
}

void Window::Dispose() {
  glfwDestroyWindow(window_);
  bgfx::resetView(view_id_);

  if(isValid(frame_buffer_)) {
    bgfx::destroy(frame_buffer_);
    frame_buffer_ = BGFX_INVALID_HANDLE;
  }

  FreeViewId(view_id_);

  view_id_ = BGFX_INVALID_HANDLE;
}

void Window::Initialize() {
  const glm::u16vec2 initial_window_resolution = GetResolution();
  view_id_ = ReserveViewId();

  if (BgfxSupportsMultipleWindows()) {
    bgfx::createFrameBuffer(GetNativeWindowHandle(window_), initial_window_resolution.x, initial_window_resolution.y, bgfx::TextureFormat::RGBA8, bgfx::TextureFormat::D24S8);
    frame_buffer_ = bgfx::createFrameBuffer(GetNativeWindowHandle(window_), initial_window_resolution.x, initial_window_resolution.y);
    bgfx::setViewFrameBuffer(view_id_, frame_buffer_);
  } else {
    BgfxInitializationScoped::GetInstance()->ReInitialize(window_, initial_window_resolution);
  }

  bgfx::setViewRect(view_id_, 0, 0, initial_window_resolution.x, initial_window_resolution.y);
  bgfx::setViewClear(view_id_, BGFX_CLEAR_COLOR, 0x000000FF);
  back_buffer_size_ = initial_window_resolution;
}

void Window::SetClearColor(std::uint32_t rgba) const {
  bgfx::setViewClear(view_id_, BGFX_CLEAR_COLOR, rgba);
}

glm::u16vec2 Window::GetSize() const {
  return big2::GetWindowSize(window_);
}

glm::u16vec2 Window::GetResolution() const {
  return big2::GetWindowResolution(window_);
}

void Window::SetFrameSize(glm::u16vec2 size) {
  back_buffer_size_ = size;
  if (BgfxSupportsMultipleWindows()) {
    bgfx::destroy(frame_buffer_);
    frame_buffer_ = bgfx::createFrameBuffer(GetNativeWindowHandle(window_), size.x, size.y);
    bgfx::setViewFrameBuffer(view_id_, frame_buffer_);
  } else {
    bgfx::reset(size.x, size.y);
  }

  bgfx::setViewRect(view_id_, 0, 0, size.x, size.y);
}

void Window::SetWindowSize(glm::u16vec2 size) {
  glfwSetWindowSize(window_, size.x, size.y);
}

bool Window::GetShouldClose() const {
  return glfwWindowShouldClose(window_);
}

glm::u16vec2 Window::GetBackBufferSize() const {
  return back_buffer_size_;
}
}
