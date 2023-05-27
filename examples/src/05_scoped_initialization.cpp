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

  glm::ivec2 window_size = big2::GetWindowSize(window);
  init_object.resolution.width = window_size.x;
  init_object.resolution.height = window_size.y;
  init_object.resolution.reset = BGFX_RESET_VSYNC;

  big2::Validate(bgfx::init(init_object), "BGFX couldn't be initialized");
  const bgfx::ViewId main_view_id = big2::ReserveViewId();

#if BIG2_IMGUI_ENABLED
  big2::ImGuiSingleContextScoped _context(window, main_view_id, /*use_default_callbacks=*/ true);

  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
  ImGui::StyleColorsDark();
#endif // BIG2_IMGUI_ENABLED

  bgfx::setViewClear(main_view_id, BGFX_CLEAR_COLOR, 0x000000FF);
  bgfx::setViewRect(main_view_id, 0, 0, window_size.x, window_size.y);

  big2::GlfwEventQueue::Initialize();
  while (!glfwWindowShouldClose(window)) {
    big2::GlfwEventQueue::PollEvents();
    const glm::ivec2 new_window_size = big2::GetWindowSize(window);
    if (new_window_size != window_size) {
      bgfx::reset(new_window_size.x, new_window_size.y, BGFX_RESET_VSYNC);
      bgfx::setViewRect(main_view_id, 0, 0, bgfx::BackbufferRatio::Equal);
      window_size = new_window_size;
    }

    bgfx::touch(main_view_id);

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
