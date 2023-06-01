//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//
#include <big2/glfw/glfw_window_scoped.h>
#include <big2/asserts.h>
#include <GLFW/glfw3.h>

namespace big2 {

GlfwWindowScoped::GlfwWindowScoped(gsl::czstring title, glm::ivec2 size, GLFWmonitor *monitor) {
  constexpr GLFWwindow *shared_window = nullptr;
  window_ = glfwCreateWindow(size.x, size.y, title, monitor, shared_window);
  big2::Validate(window_ != nullptr, "Window couldn't be created!");
}

GlfwWindowScoped::~GlfwWindowScoped() {
  glfwDestroyWindow(window_);
}

GlfwWindowScoped::GlfwWindowScoped(gsl::not_null<GLFWwindow *> window) : window_(window) {

}

}
