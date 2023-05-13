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

  GLFWwindow *window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+BGFX example", nullptr, nullptr);
  Ensures(window != nullptr);

  gsl::final_action destroy_window([window]() { glfwDestroyWindow(window); });

  bgfx::Init init_object;
  big2::SetNativeWindowData(init_object, window);

  glm::ivec2 window_size = big2::GetWindowSize(window);
  init_object.resolution.width = window_size.x;
  init_object.resolution.height = window_size.y;
  init_object.resolution.reset = BGFX_RESET_VSYNC;

  Ensures(bgfx::init(init_object));
  const bgfx::ViewId main_view_id = 0;

#if BIG2_IMGUI_ENABLED

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  big2::ImGuiInit(window, main_view_id);

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
  // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
  // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
  // - Read 'docs/FONTS.md' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
  // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
  //io.Fonts->AddFontDefault();
  //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
  //IM_ASSERT(font != NULL);

  // Our state
  bool show_demo_window = true;

  gsl::final_action terminate_imgui([]() {
    big2::ImGuiTerminate();
    ImGui::DestroyContext();
  });

#endif // BIG2_IMGUI_ENABLED

  bgfx::setViewClear(main_view_id, BGFX_CLEAR_COLOR, 0x000000FF);
  bgfx::setViewRect(main_view_id, 0, 0, window_size.x, window_size.y);

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    glfwPollEvents();

    const glm::ivec2 new_window_size = big2::GetWindowSize(window);
    if (new_window_size != window_size) {
      bgfx::reset(new_window_size.x, new_window_size.y, BGFX_RESET_VSYNC);
      bgfx::setViewRect(main_view_id, 0, 0, bgfx::BackbufferRatio::Equal);
      window_size = new_window_size;
    }

    std::int32_t display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    bgfx::reset(display_w, display_h, BGFX_RESET_VSYNC);
    bgfx::setViewRect(main_view_id, 0, 0, bgfx::BackbufferRatio::Equal);

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
