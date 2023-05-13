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

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

static void glfw_error_callback(int error, const char *description) {
  std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

// Main code
int main(int, char **) {
  glfwSetErrorCallback(glfw_error_callback);
  Ensures(glfwInit());

  gsl::final_action terminate_glfw([]() { glfwTerminate(); });

  bgfx::Init init_object;
  big2::SetNativeData(init_object);
  Ensures(bgfx::init(init_object));

  std::array<GLFWwindow *, 2> windows{};
  std::array<bgfx::ViewId, 2> views{};
  std::array<bgfx::FrameBufferHandle, 2> frame_buffers{};
  for (std::uint32_t i = 0; i < windows.size(); i++) {
    std::string title = "Window " + std::to_string(i);
    windows[i] = glfwCreateWindow(800, 600, title.c_str(), nullptr, nullptr);
    Ensures(windows[i] != nullptr);

    views[i] = i;
    frame_buffers[i] = big2::CreateWindowFramebuffer(windows[i]);
    bgfx::setViewFrameBuffer(views[i], frame_buffers[i]);
    glm::ivec2 window_size = big2::GetWindowSize(windows[i]);

    bgfx::setViewRect(views[i], 0, 0, static_cast<std::uint16_t>(window_size.x), static_cast<std::uint16_t>(window_size.y));
  }

  gsl::final_action terminate_windows([&frame_buffers, &windows]() {
    std::for_each(frame_buffers.begin(), frame_buffers.end(), [](bgfx::FrameBufferHandle fbh) { bgfx::destroy(fbh); });
    std::for_each(windows.begin(), windows.end(), [](GLFWwindow *w) { glfwDestroyWindow(w); });
  });

  bgfx::setViewClear(views[0], BGFX_CLEAR_COLOR, 0x993399FF);
  bgfx::setViewClear(views[1], BGFX_CLEAR_COLOR, 0x333399FF);

  // Main loop
  while (std::none_of(windows.begin(), windows.end(), [](GLFWwindow *window) { return glfwWindowShouldClose(window); })) {
    glfwPollEvents();
    std::for_each(views.begin(), views.end(), [](bgfx::ViewId view_id) { bgfx::touch(view_id); });
    bgfx::frame();
  }

  return 0;
}
