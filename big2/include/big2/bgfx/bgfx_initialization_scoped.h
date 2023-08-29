//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_BGFX_INITIALIZATION_SCOPED_H_
#define BIG2_STACK_BGFX_INITIALIZATION_SCOPED_H_

#include <bgfx/bgfx.h>

namespace big2 {

/**
 * @brief A scoped initialization class for BGFX.
 * @details This class will warn if terminated or initialized twice and will also set
 * a default error callback that will log any errors.
 * @see BIG2_SCOPE(assignment)
 */
class BgfxInitializationScoped final {
 public:
  BgfxInitializationScoped(bgfx::RendererType::Enum renderer_type = bgfx::RendererType::Count);

  BgfxInitializationScoped(BgfxInitializationScoped &&) = default;

  BgfxInitializationScoped &operator=(BgfxInitializationScoped &&) = default;

  BgfxInitializationScoped(const BgfxInitializationScoped &) = delete;

  BgfxInitializationScoped &operator=(const BgfxInitializationScoped &) = delete;

  ~BgfxInitializationScoped();
};

}

#endif //BIG2_STACK_BGFX_INITIALIZATION_SCOPED_H_
