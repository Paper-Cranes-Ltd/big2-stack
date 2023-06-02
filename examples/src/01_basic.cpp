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
  glfwSetErrorCallback(big2::GlfwErrorCallback);
  big2::Validate(glfwInit() == GLFW_TRUE, "GLFW couldn't be initialized!");

  gsl::final_action terminate_glfw([]() { glfwTerminate(); });

  GLFWwindow *window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+BGFX example", nullptr, nullptr);
  big2::Validate(window != nullptr, "Window handle is nullptr");

  gsl::final_action destroy_window([window]() { glfwDestroyWindow(window); });

  bgfx::Init init_object;
  big2::SetNativeWindowData(init_object, window);

  glm::ivec2 window_resolution = big2::GetWindowResolution(window);
  init_object.resolution.width = window_resolution.x;
  init_object.resolution.height = window_resolution.y;
  init_object.resolution.reset = BGFX_RESET_VSYNC;

  big2::Validate(bgfx::init(init_object), "BGFX couldn't be initialized");
  const bgfx::ViewId main_view_id = big2::ReserveViewId();

#if BIG2_IMGUI_ENABLED
  big2::ImGuiInit(window, main_view_id, /*use_default_callbacks=*/ true);
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
  ImGui::StyleColorsDark();

  bool show_demo_window = true;

  gsl::final_action terminate_imgui([]() {
    big2::ImGuiTerminate();
  });

#endif // BIG2_IMGUI_ENABLED

  bgfx::setViewClear(main_view_id, BGFX_CLEAR_COLOR, 0x000000FF);
  bgfx::setViewRect(main_view_id, 0, 0, window_resolution.x, window_resolution.y);

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    const glm::ivec2 new_window_resolution = big2::GetWindowResolution(window);
    if (new_window_resolution != window_resolution) {
      bgfx::reset(new_window_resolution.x, new_window_resolution.y, BGFX_RESET_VSYNC);
      bgfx::setViewRect(main_view_id, 0, 0, new_window_resolution.x, new_window_resolution.y);
      window_resolution = new_window_resolution;
    }

    bgfx::touch(main_view_id);

#if BIG2_IMGUI_ENABLED
    // Start the Dear ImGui frame
    big2::ImGuiBeginFrame();

    if (show_demo_window) {
      ImGui::ShowDemoWindow(&show_demo_window);
    }

    big2::ImGuiEndFrame();
#endif // BIG2_IMGUI_ENABLED

    bgfx::frame();
  }

  return 0;
}
