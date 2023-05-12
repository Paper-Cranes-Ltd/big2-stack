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

namespace big2 {

/**
 * @brief Adds native display data to the initialization object.
 * @details Use this in case you have a multi-window project
 * and you would like to initialize separate frame buffers for each window.
 * @param init_obj Uninitialized bgfx init object
 */
void SetNativeData(bgfx::Init &init_obj);

/**
 * @brief Adds native display and window data to the initialization object.
 * @details Use this in case you have a single-window project.
 * @param init_obj Uninitialized bgfx init object
 * @param window An initialized window handle
 */
void SetNativeWindowData(bgfx::Init &init_obj, gsl::not_null<GLFWwindow *> window);

/**
 * @brief Creates a frame buffer for the given window.
 * @param window An initialized window handle
 * @return A handle to the frame buffer. You would likely need to link this to a bgfx::ViewId through bgfx::setViewFrameBuffer
 */
[[nodiscard]] bgfx::FrameBufferHandle CreateWindowFramebuffer(gsl::not_null<GLFWwindow *> window);
}

#endif //BIG2_STACK_BIG2_BGFX_UTILS_H_
