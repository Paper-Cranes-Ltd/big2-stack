//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_BIG2_GLFW_UTILS_H_
#define BIG2_STACK_BIG2_GLFW_UTILS_H_

#include <gsl/span>
#include <bgfx/bgfx.h>

struct GLFWwindow;
struct GLFWmonitor;

namespace big2
{
void SetNativeWindowData(bgfx::Init &init_obj, GLFWwindow *window);
gsl::span<GLFWmonitor*> GetMonitors();
}

#endif //BIG2_STACK_BIG2_GLFW_UTILS_H_
