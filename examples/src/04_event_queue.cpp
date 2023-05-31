// Dear ImGui: standalone example application for GLFW + BGFX, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// (BGFX is a corss-platfrom general purpose rendering library with multiple backends like OpenGL, Vulkan, Metal, Gnm etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include <big2.h>

#include <iostream>
#include <string>

#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <imgui.h>

#include <GLFW/glfw3.h>

struct WindowData {
  bgfx::ViewId view_id = BGFX_INVALID_HANDLE;
  bgfx::FrameBufferHandle frame_buffer_handle = BGFX_INVALID_HANDLE;
  GLFWwindow *window = nullptr;
  ImGuiContext *imgui_context = nullptr;
};

int main(std::int32_t, gsl::zstring[]) {
  glfwSetErrorCallback(big2::GlfwErrorCallback);
  big2::Validate(glfwInit() == GLFW_TRUE, "GLFW couldn't be initialized!");

  gsl::final_action terminate_glfw([]() { glfwTerminate(); });

  bgfx::Init init_object;
  big2::SetNativeData(init_object);
  big2::Validate(bgfx::init(init_object), "BGFX couldn't be initialized");

  big2::GlfwEventQueue::Initialize();

  IMGUI_CHECKVERSION();

  std::array<WindowData, 2> window_data_s;
  for (std::uint32_t i = 0; i < window_data_s.size(); i++) {
    std::string title = "Window " + std::to_string(i);
    window_data_s[i].window = glfwCreateWindow(800, 600, title.c_str(), nullptr, nullptr);
    big2::Validate(window_data_s[i].window != nullptr, "Window handle is nullptr");

    big2::GlfwEventQueue::ConnectWindow(window_data_s[i].window);
    window_data_s[i].view_id = big2::ReserveViewId();
    window_data_s[i].frame_buffer_handle = big2::CreateWindowFramebuffer(window_data_s[i].window);
    bgfx::setViewFrameBuffer(window_data_s[i].view_id, window_data_s[i].frame_buffer_handle);
    glm::ivec2 window_resolution = big2::GetWindowResolution(window_data_s[i].window);

    bgfx::setViewRect(window_data_s[i].view_id, 0, 0, static_cast<std::uint16_t>(window_resolution.x), static_cast<std::uint16_t>(window_resolution.y));

#if BIG2_IMGUI_ENABLED
    window_data_s[i].imgui_context = big2::ImGuiInit(window_data_s[i].window, window_data_s[i].view_id);
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    ImGui::StyleColorsDark();
#endif // BIG2_IMGUI_ENABLED
  }

  gsl::final_action terminate_windows([&window_data_s]() {
    std::for_each(window_data_s.begin(), window_data_s.end(),
                  [](const WindowData &data) {
                    big2::ImGuiTerminate(data.imgui_context);
                    bgfx::destroy(data.frame_buffer_handle);
                    bgfx::resetView(data.view_id);
                    big2::FreeViewId(data.view_id);
                    glfwDestroyWindow(data.window);
                  });
  });

  bgfx::setViewClear(window_data_s[0].view_id, BGFX_CLEAR_COLOR, 0x993399FF);
  bgfx::setViewClear(window_data_s[1].view_id, BGFX_CLEAR_COLOR, 0x333399FF);

  // Main loop
  while (std::none_of(window_data_s.begin(), window_data_s.end(), [](const WindowData &data) { return glfwWindowShouldClose(data.window); })) {
    big2::GlfwEventQueue::PollEvents();

    for (const WindowData &data : window_data_s) {
      gsl::span<big2::GlfwEvent> events = big2::GlfwEventQueue::GrabEvents(data.window);

#if BIG2_IMGUI_ENABLED
      ImGui::SetCurrentContext(data.imgui_context);
      big2::GlfwEventQueue::UpdateImGuiEvents(data.window, events);

      big2::ImGuiBeginFrame();
      ImGui::ShowDemoWindow();
      big2::ImGuiEndFrame();
#endif //  BIG2_IMGUI_ENABLED

      bgfx::touch(data.view_id);
    }

    bgfx::frame();
  }

  return 0;
}
