// Dear ImGui: standalone example application for GLFW + BGFX, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// (BGFX is a corss-platfrom general purpose rendering library with multiple backends like OpenGL, Vulkan, Metal, Gnm etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include <big2.h>

#if BIG2_IMGUI_ENABLED
#include <imgui.h>
#endif // BIG2_IMGUI_ENABLED

#include <iostream>

#include <bgfx/bgfx.h>
#include <bx/bx.h>

#include <GLFW/glfw3.h>

int main(std::int32_t, gsl::zstring[]) {
  big2::GlfwInitializationScoped _;
  big2::GlfwWindowScoped window("Dear ImGui GLFW+BGFX example", {1280, 720});

  bgfx::Init init_object;
  big2::SetNativeWindowData(init_object, window);

  glm::ivec2 window_resolution = big2::GetWindowResolution(window);
  init_object.resolution.width = window_resolution.x;
  init_object.resolution.height = window_resolution.y;
  init_object.resolution.reset = BGFX_RESET_VSYNC;

  big2::Validate(bgfx::init(init_object), "BGFX couldn't be initialized");

  const big2::BgfxViewScoped main_view;

#if BIG2_IMGUI_ENABLED
  big2::ImGuiSingleContextScoped _context(window, main_view, /*use_default_callbacks=*/ true);

  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
  ImGui::StyleColorsDark();
#endif // BIG2_IMGUI_ENABLED

  bgfx::setViewClear(main_view, BGFX_CLEAR_COLOR, 0x000000FF);
  bgfx::setViewRect(main_view, 0, 0, window_resolution.x, window_resolution.y);

  big2::GlfwEventQueue::Initialize();
  while (!glfwWindowShouldClose(window)) {
    big2::GlfwEventQueue::PollEvents();
    const glm::ivec2 new_window_size = big2::GetWindowSize(window);
    if (new_window_size != window_resolution) {
      bgfx::reset(new_window_size.x, new_window_size.y, BGFX_RESET_VSYNC);
      bgfx::setViewRect(main_view, 0, 0, bgfx::BackbufferRatio::Equal);
      window_resolution = new_window_size;
    }

    bgfx::touch(main_view);

#if BIG2_IMGUI_ENABLED
    big2::GlfwEventQueue::UpdateImGuiEvents(window);
    BIG2_SCOPE_VAR(big2::ImGuiFrameScoped) {
      ImGui::ShowDemoWindow();
    }
#endif // BIG2_IMGUI_ENABLED

    bgfx::frame();
  }

  return 0;
}
