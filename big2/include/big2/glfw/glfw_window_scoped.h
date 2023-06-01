//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_GLFW_WINDOW_SCOPED_H
#define BIG2_STACK_GLFW_WINDOW_SCOPED_H

#include <gsl/gsl>
#include <glm/glm.hpp>

struct GLFWwindow;
struct GLFWmonitor;

namespace big2 {

/**
 * @brief This class will wrap a GLFWwindow pointer and dispose it upon destruction.
 * @details You could put this in an unique_ptr to have it for longer and pass it on to be owned and disposed properly.
 * Otherwise you could use it with BIG2_SCOPE or just a normal scope.
 * @see BIG2_SCOPE(assignment)
 */
class GlfwWindowScoped final {
 public:
  explicit(false) GlfwWindowScoped(gsl::not_null<GLFWwindow *> window);
  GlfwWindowScoped(gsl::czstring title, glm::ivec2 size, GLFWmonitor *monitor = nullptr);
  GlfwWindowScoped(GlfwWindowScoped &&) = default;
  GlfwWindowScoped &operator=(GlfwWindowScoped &&) = default;
  GlfwWindowScoped(const GlfwWindowScoped &) = delete;
  GlfwWindowScoped &operator=(const GlfwWindowScoped &) = delete;
  ~GlfwWindowScoped();

  explicit(false) operator GLFWwindow *() const { return window_; }
  explicit(false) operator gsl::not_null<GLFWwindow *>() const { return window_; }

  [[nodiscard]] gsl::not_null<GLFWwindow *> GetWindow() const { return window_; }

 private:
  gsl::owner<GLFWwindow *> window_ = nullptr;
};

}

#endif //BIG2_STACK_GLFW_WINDOW_SCOPED_H
