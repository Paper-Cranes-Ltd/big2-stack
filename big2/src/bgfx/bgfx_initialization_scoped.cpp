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
BgfxInitializationScoped *BgfxInitializationScoped::instance_ = nullptr;

BgfxInitializationScoped::BgfxInitializationScoped(bgfx::RendererType::Enum renderer_type)
  : renderer_type_(renderer_type) {
  Expects(instance_ == nullptr);
  instance_ = this;

  bgfx::Init init_object = bgfx::Init();
  init_object.callback = &global_bgfx_callback_handler;
  init_object.type = renderer_type;
  init_object.resolution.width = 0;
  init_object.resolution.height = 0;
  init_object.capabilities = BGFX_CAPS_INDEX32;

  if (renderer_type == bgfx::RendererType::Vulkan) {
    big2::Validate(glfwVulkanSupported(), "Vulkan is not supported by GLFW");
  }

  big2::Validate(bgfx::init(init_object), "BGFX couldn't be initialized");
}

BgfxInitializationScoped::BgfxInitializationScoped(BgfxInitializationScoped && other) : renderer_type_(std::move(other.renderer_type_)) {
  instance_ = this;
}

BgfxInitializationScoped & BgfxInitializationScoped::operator=(BgfxInitializationScoped &&other) noexcept {
  instance_ = this;
  renderer_type_ = other.renderer_type_;
  return *this;
}

BgfxInitializationScoped::~BgfxInitializationScoped() {
  bgfx::shutdown();
}

void BgfxInitializationScoped::ReInitialize(gsl::not_null<GLFWwindow *> window, const glm::ivec2 size) {
  bgfx::shutdown();

  bgfx::Init init_object = bgfx::Init();
  init_object.callback = &global_bgfx_callback_handler;
  init_object.type = renderer_type_;
  init_object.resolution.width = size.x;
  init_object.resolution.height = size.y;
  init_object.capabilities = BGFX_CAPS_INDEX32;
  big2::SetNativeWindowData(init_object, window);

  big2::Validate(bgfx::init(init_object), "BGFX couldn't be initialized");
}
}
