//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_BIG2_BGFX_UTILS_H_
#define BIG2_STACK_BIG2_BGFX_UTILS_H_

#include <bgfx/bgfx.h>
#include <glm/glm.hpp>
#include <gsl/gsl>
#include <vector>

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
 * @brief A scoped wrapper to free and reset the view after it is finished.
 * @details You could put this in an unique_ptr to have it for longer and pass it on to be owned and disposed properly.
 * Otherwise you could use it with BIG2_SCOPE or just a normal scope.
 * @see BIG2_SCOPE(assignment)
 */
class BgfxViewScoped final {
 public:
  BgfxViewScoped();
  explicit(false) BgfxViewScoped(bgfx::ViewId view_id);
  BgfxViewScoped(BgfxViewScoped &&) = default;
  BgfxViewScoped &operator=(BgfxViewScoped &&) = default;
  BgfxViewScoped(const BgfxViewScoped &) = delete;
  BgfxViewScoped &operator=(const BgfxViewScoped &) = delete;
  ~BgfxViewScoped();

  explicit(false) operator bgfx::ViewId() const;

 private:
  bgfx::ViewId view_id_ = BGFX_INVALID_HANDLE;
};

/**
 * @brief A scoped wrapper to dispose of the frame buffer after it is finished.
 * @details You could put this in an unique_ptr to have it for longer and pass it on to be owned and disposed properly.
 * Otherwise you could use it with BIG2_SCOPE or just a normal scope.
 * @see BIG2_SCOPE(assignment)
 */
class BgfxFrameBufferScoped final {
 public:
  explicit BgfxFrameBufferScoped(gsl::not_null<GLFWwindow *> window);
  explicit(false) BgfxFrameBufferScoped(bgfx::FrameBufferHandle handle);
  BgfxFrameBufferScoped(BgfxFrameBufferScoped &&) = default;
  BgfxFrameBufferScoped &operator=(BgfxFrameBufferScoped &&) = default;
  BgfxFrameBufferScoped(const BgfxFrameBufferScoped &) = delete;
  BgfxFrameBufferScoped &operator=(const BgfxFrameBufferScoped &) = delete;
  ~BgfxFrameBufferScoped();

  explicit(false) operator bgfx::FrameBufferHandle() const;
  explicit(false) operator bgfx::FrameBufferHandle&();

 private:
  bgfx::FrameBufferHandle handle_ = BGFX_INVALID_HANDLE;
};

}

#endif //BIG2_STACK_BIG2_BGFX_UTILS_H_
