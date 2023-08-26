//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//
#include <big2/bgfx/bgfx_initialization_scoped.h>
#include <big2/macros.h>
#include <big2/asserts.h>
#include <bgfx/bgfx.h>
#include <big2/bgfx/bgfx_utils.h>
#include <big2/bgfx/bgfx_callback_handler.h>

namespace big2 {

static BgfxCallbackHandler global_bgfx_callback_handler;

BgfxInitializationScoped::BgfxInitializationScoped(bgfx::RendererType::Enum renderer_type) {
  bgfx::Init init_object = bgfx::Init();
  init_object.callback = &global_bgfx_callback_handler;
  init_object.type = renderer_type;
  init_object.resolution.width = 0;
  init_object.resolution.height = 0;
  init_object.capabilities = BGFX_CAPS_SWAP_CHAIN | BGFX_CAPS_DRAW_INDIRECT | BGFX_CAPS_FRAGMENT_DEPTH;

  big2::Validate(bgfx::init(init_object), "BGFX couldn't be initialized");
}

}
