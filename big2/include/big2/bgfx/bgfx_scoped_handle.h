//
// Copyright (c) 2024 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BGFX_SCOPED_HANDLE_H
#define BGFX_SCOPED_HANDLE_H

namespace big2 {
template<typename BgfxType>
struct BgfxScopedHandle {
  BgfxScopedHandle() = default;
  explicit(false) BgfxScopedHandle(BgfxType handle)
    : handle_(handle) {
  }
  BgfxScopedHandle &operator=(const BgfxType &handle) {
    handle_ = handle;
    return *this;
  }
  BgfxScopedHandle(const BgfxScopedHandle &) = delete;
  BgfxScopedHandle &operator=(const BgfxScopedHandle &) = delete;
  BgfxScopedHandle(BgfxScopedHandle &&other) noexcept
    : handle_(other.handle_) {
    other.handle_ = BGFX_INVALID_HANDLE;
  }
  BgfxScopedHandle &operator=(BgfxScopedHandle &&other) noexcept {
    handle_ = other.handle_;
    other.handle_ = BGFX_INVALID_HANDLE;
    return *this;
  }

  ~BgfxScopedHandle() {
    Destroy();
  }

  explicit(false) operator BgfxType() const {
    return handle_;
  }

  explicit(false) operator BgfxType &() {
    return handle_;
  }

  [[nodiscard]] bool IsValid() const { return bgfx::isValid(handle_); }

  void Destroy() {
    if (IsValid()) {
      bgfx::destroy(handle_);
      handle_ = BGFX_INVALID_HANDLE;
    }
  }

  BgfxType handle_ = BGFX_INVALID_HANDLE;
};

using DynamicIndexBufferScopedHandle = BgfxScopedHandle<bgfx::DynamicIndexBufferHandle>;
using DynamicVertexBufferScopedHandle = BgfxScopedHandle<bgfx::DynamicVertexBufferHandle>;
using FrameBufferScopedHandle = BgfxScopedHandle<bgfx::FrameBufferHandle>;
using IndexBufferScopedHandle = BgfxScopedHandle<bgfx::IndexBufferHandle>;
using IndirectBufferScopedHandle = BgfxScopedHandle<bgfx::IndirectBufferHandle>;
using OcclusionQueryScopedHandle = BgfxScopedHandle<bgfx::OcclusionQueryHandle>;
using ProgramScopedHandle = BgfxScopedHandle<bgfx::ProgramHandle>;
using ShaderScopedHandle = BgfxScopedHandle<bgfx::ShaderHandle>;
using TextureScopedHandle = BgfxScopedHandle<bgfx::TextureHandle>;
using UniformScopedHandle = BgfxScopedHandle<bgfx::UniformHandle>;
using VertexBufferScopedHandle = BgfxScopedHandle<bgfx::VertexBufferHandle>;
using VertexLayoutScopedHandle = BgfxScopedHandle<bgfx::VertexLayoutHandle>;
}

#endif //BGFX_SCOPED_HANDLE_H
