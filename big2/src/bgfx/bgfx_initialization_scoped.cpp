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
#include <bgfx/platform.h>

namespace big2 {

static BgfxCallbackHandler global_bgfx_callback_handler;

BgfxInitializationScoped::BgfxInitializationScoped(bgfx::RendererType::Enum renderer_type) {

  // Use single-threaded rendering!
  bgfx::renderFrame();

  bgfx::Init init_object = bgfx::Init();
  init_object.callback = &global_bgfx_callback_handler;
  init_object.type = renderer_type;
  init_object.resolution.width = 0;
  init_object.resolution.height = 0;
  init_object.capabilities = BGFX_CAPS_SWAP_CHAIN | BGFX_CAPS_INDEX32;

  if(renderer_type == bgfx::RendererType::Vulkan) {
    big2::Validate(glfwVulkanSupported(), "Vulkan is not supported by GLFW");
  }

  big2::Validate(bgfx::init(init_object), "BGFX couldn't be initialized");
}

}
