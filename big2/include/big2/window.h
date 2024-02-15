//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_GLFW_WINDOW_SCOPED_H
#define BIG2_STACK_GLFW_WINDOW_SCOPED_H

#include <gsl/gsl>
#include <glm/glm.hpp>
#include <bgfx/bgfx.h>

struct GLFWwindow;
struct GLFWmonitor;

namespace big2 {

/**
 * @brief This class will wrap a GLFWwindow pointer and dispose it upon destruction.
 * @details You could put this in an unique_ptr to have it for longer and pass it on to be owned and disposed properly.
 * Otherwise you could use it with BIG2_SCOPE or just a normal scope.
 * @see BIG2_SCOPE(assignment)
 */
class Window final {
 public:
  explicit(false) Window(gsl::not_null<GLFWwindow *> window);
  Window(gsl::czstring title, glm::ivec2 size, GLFWmonitor *monitor = nullptr);
  Window(Window &&) = default;
  Window &operator=(Window &&) = default;
  Window(const Window &) = default;
  Window &operator=(const Window &) = default;
  ~Window();

  explicit(false) operator GLFWwindow *() const { return window_; }
  explicit(false) operator gsl::not_null<GLFWwindow *>() const { return window_; }

  void SetClearColor(std::uint32_t rgba) const;
  void SetFrameSize(glm::ivec2 size);
  void SetWindowSize(glm::ivec2 size);
  void Dispose();

  [[nodiscard]] gsl::not_null<GLFWwindow *> GetWindowHandle() const { return window_; }
  [[nodiscard]] bool GetIsScoped() const { return is_scoped_; }
  Window& SetIsScoped(bool scoped);
  [[nodiscard]] bool GetIsResizable() const;
  Window& SetIsResizable(bool is_resizable);
  [[nodiscard]] bgfx::ViewId GetView() const { return view_id_; }
  [[nodiscard]] bgfx::FrameBufferHandle GetFrameBuffer() const { return frame_buffer_; }
  [[nodiscard]] glm::ivec2 GetSize() const;
  [[nodiscard]] glm::ivec2 GetResolution() const;
  [[nodiscard]] bool GetShouldClose() const;

 private:
  void Initialize();

  gsl::owner<GLFWwindow *> window_ = nullptr;
  bgfx::FrameBufferHandle frame_buffer_ = BGFX_INVALID_HANDLE;
  bgfx::ViewId view_id_ = BGFX_INVALID_HANDLE;
  bool is_scoped_ = true;
};

}

#endif //BIG2_STACK_GLFW_WINDOW_SCOPED_H
