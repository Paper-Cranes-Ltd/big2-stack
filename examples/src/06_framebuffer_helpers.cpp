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
#include <ranges>

int main(std::int32_t, gsl::zstring[]) {
  big2::GlfwInitializationScoped _;
  big2::GlfwWindowScoped window("Dear ImGui GLFW+BGFX example", {1280, 720});

  bgfx::Init init_object;
  big2::SetNativeData(init_object);
  big2::Validate(bgfx::init(init_object), "BGFX couldn't be initialized");

  const bgfx::ViewId main_view_id = big2::ReserveViewId();
  bgfx::FrameBufferHandle frame_buffer_handle = big2::CreateWindowFramebuffer(window);
  bgfx::setViewFrameBuffer(main_view_id, frame_buffer_handle);
  glm::ivec2 window_resolution = big2::GetWindowResolution(window);
  bgfx::setViewRect(main_view_id, 0, 0, window_resolution.x, window_resolution.y);
  bgfx::setViewClear(main_view_id, BGFX_CLEAR_COLOR, 0x000000FF);

#if BIG2_IMGUI_ENABLED
  big2::ImGuiSingleContextScoped _context(window, main_view_id, /*use_default_callbacks=*/ true);

  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
  ImGui::StyleColorsDark();
#endif // BIG2_IMGUI_ENABLED

  big2::GlfwEventQueue::Initialize();
  while (!glfwWindowShouldClose(window)) {
    big2::GlfwEventQueue::PollEvents();

    gsl::span<big2::GlfwEvent> events = big2::GlfwEventQueue::GrabEvents(window);
    auto frame_buffer_updated_event = [](big2::GlfwEvent& event) { return event.Is<big2::GlfwEvent::FrameBufferResized>(); };
    if(std::find_if(events.begin(), events.end(), frame_buffer_updated_event) != events.end())
    {
      window_resolution = big2::GetWindowResolution(window);
      big2::UpdateFrameBuffer(window, frame_buffer_handle);
      bgfx::setViewFrameBuffer(main_view_id, frame_buffer_handle);
      bgfx::setViewRect(main_view_id, 0, 0, window_resolution.x, window_resolution.y);
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
