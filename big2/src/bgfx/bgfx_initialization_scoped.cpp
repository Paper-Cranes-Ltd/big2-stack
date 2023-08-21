//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//
#include <big2/bgfx/bgfx_initialization_scoped.h>
#include <big2/macros.h>
#include <big2/asserts.h>
#include <bgfx/bgfx.h>
#include <big2/bgfx/bgfx_utils.h>

namespace big2 {

BgfxInitializationScoped::BgfxInitializationScoped(bgfx::RendererType::Enum renderer_type) {
  bgfx::Init init_object;
  init_object.type = renderer_type;
  init_object.resolution.reset = BGFX_RESET_VSYNC | BGFX_RESET_DEPTH_CLAMP;
  big2::SetNativeData(init_object);
  big2::Validate(bgfx::init(init_object), "BGFX couldn't be initialized");
}

}
