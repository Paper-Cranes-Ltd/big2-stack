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

BgfxInitializationScoped::BgfxInitializationScoped() {
  bgfx::Init init_object;
  big2::SetNativeData(init_object);
  big2::Validate(bgfx::init(init_object), "BGFX couldn't be initialized");
}

}
