//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_BIG2_BGFX_UTILS_H_
#define BIG2_STACK_BIG2_BGFX_UTILS_H_

#include <bgfx/bgfx.h>
#include <glm/glm.hpp>
#include <gsl/gsl>

struct GLFWwindow;

namespace big2
{
void SetNativeData(bgfx::Init &init_obj);
void SetNativeWindowData(bgfx::Init &init_obj, gsl::not_null<GLFWwindow *> window);
[[nodiscard]] bgfx::FrameBufferHandle CreateWindowFramebuffer(gsl::not_null<GLFWwindow *> window);
}

#endif //BIG2_STACK_BIG2_BGFX_UTILS_H_
