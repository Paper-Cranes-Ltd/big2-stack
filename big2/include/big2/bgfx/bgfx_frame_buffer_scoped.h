//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_BGFX_FRAME_BUFFER_SCOPED_H_
#define BIG2_STACK_BGFX_FRAME_BUFFER_SCOPED_H_

#include <gsl/gsl>
#include <bgfx/bgfx.h>
#include <GLFW/glfw3.h>

namespace big2 {

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
  explicit(false) operator bgfx::FrameBufferHandle &();

 private:
  bgfx::FrameBufferHandle handle_ = BGFX_INVALID_HANDLE;
};

}

#endif //BIG2_STACK_BGFX_FRAME_BUFFER_SCOPED_H_
