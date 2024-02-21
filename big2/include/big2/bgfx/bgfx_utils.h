//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_BGFX_UTILS_H_
#define BIG2_STACK_BGFX_UTILS_H_

#include <bgfx/bgfx.h>
#include <glm/glm.hpp>
#include <gsl/gsl>
#include <vector>
#include <big2/bgfx/bgfx_frame_buffer_scoped.h>
#include <big2/bgfx/bgfx_view_scoped.h>
#include <big2/bgfx/bgfx_initialization_scoped.h>

struct GLFWwindow;

namespace big2 {

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
[[nodiscard]] bgfx::FrameBufferHandle CreateWindowFrameBuffer(gsl::not_null<GLFWwindow *> window);

/**
 * @brief Updates an existing framebuffer handle by destroying the current framebuffer and then recreating it.
 * @param window An initialized window handle
 * @param out_handle A handle to the frame buffer.
 */
void ResetWindowFrameBuffer(gsl::not_null<GLFWwindow *> window, bgfx::FrameBufferHandle &out_handle);

/**
 * @brief BIG2 will use an IdManager class to reserve and monitor ViewIds
 * @return The first free ViewId (that isn't reserved by this function)
 */
[[nodiscard]] bgfx::ViewId ReserveViewId();

/**
 * @brief Get the ViewIds that were reserved by BIG2's ReserveViewId()
 * @return A vector of ViewId values that were previously reserved
 */
[[nodiscard]] std::vector<bgfx::ViewId> GetReservedViewIds();

/**
 * @brief Frees previously reserved ViewId
 * @param The ViewId that will be freed
 */
void FreeViewId(bgfx::ViewId value);

/**
 * \brief Checks after initialization if bgfx is capable of rendering to multiple windows.
 */
bool BgfxSupportsMultipleWindows();


}

#endif //BIG2_STACK_BGFX_UTILS_H_
