//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//
#include <big2/glfw/glfw_initialization_scoped.h>
#include <big2/macros.h>
#include <big2/glfw/glfw_utils.h>
#include <big2/asserts.h>
#include <GLFW/glfw3.h>

namespace big2 {

GlfwInitializationScoped::GlfwInitializationScoped() {
  if (is_initialized_) {
    big2::Warning("GLFW is initialized twice");
    return;
  }

  glfwSetErrorCallback(GlfwErrorCallback);

  is_initialized_ = glfwInit() == GLFW_TRUE;
  big2::Validate(is_initialized_, "Couldn't initialize GLFW");

  glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

GlfwInitializationScoped::~GlfwInitializationScoped() {
  if (!is_initialized_) {
    big2::Warning("GLFW is uninitialized twice");
    return;
  }

  glfwTerminate();
  is_initialized_ = false;
}

}
