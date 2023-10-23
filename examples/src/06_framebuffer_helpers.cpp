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
  big2::GlfwInitializationScoped _glfw;
  big2::BgfxInitializationScoped _bgfx;

  big2::Window window("Dear ImGui GLFW+BGFX example", {1280, 720});
  big2::GlfwEventQueue::ConnectWindow(window);

#if BIG2_IMGUI_ENABLED
  big2::ImGuiContextWrapper imgui_context;

  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
  imgui_context.Initialize(window, window.GetView(), true);

  ImGui::StyleColorsDark();
#endif // BIG2_IMGUI_ENABLED

  big2::GlfwEventQueue::Initialize();

  while (!glfwWindowShouldClose(window)) {
    big2::GlfwEventQueue::PollEvents();

    if (big2::GlfwEventQueue::HasEventType<big2::GlfwEvent::FrameBufferResized>(window)) {
      const glm::ivec2 window_resolution = window.GetResolution();
      window.SetFrameSize(window_resolution);
    }

    bgfx::touch(window.GetView());

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
