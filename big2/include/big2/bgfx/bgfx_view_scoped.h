//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_BGFX_VIEW_SCOPED_H_
#define BIG2_STACK_BGFX_VIEW_SCOPED_H_

#include <bgfx/bgfx.h>

namespace big2 {

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

}

#endif //BIG2_STACK_BGFX_VIEW_SCOPED_H_
