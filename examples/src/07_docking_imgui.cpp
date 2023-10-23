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
#include <big2/bgfx/embedded_shader.h>
#include <generated/shaders/examples/all.h>

static const bgfx::EmbeddedShader kEmbeddedShaders[] =
    {
        BGFX_EMBEDDED_SHADER(vs_basic),
        BGFX_EMBEDDED_SHADER(fs_basic),
        BGFX_EMBEDDED_SHADER_END()
    };

struct NormalColorVertex {
  glm::vec2 position;
  uint32_t color;
};

int main(std::int32_t, gsl::zstring[]) {
  big2::GlfwInitializationScoped _glfw;
  big2::BgfxInitializationScoped _bgfx;

//  glfwWindowHint(GLFW_DECORATED, false);
  glfwWindowHint(GLFW_FLOATING, false);
  big2::Window window("Dear ImGui GLFW+BGFX example", {1280, 720});

#if BIG2_IMGUI_ENABLED
  big2::ImGuiContextWrapper imgui_context;

  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

#if defined(IMGUI_HAS_DOCK)
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable docking
#endif // defined(IMGUI_HAS_DOCK)

#if defined(IMGUI_HAS_VIEWPORT)
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // Enable viewports
#endif // defined(IMGUI_HAS_VIEWPORT)
  imgui_context.Initialize(window, window.GetView(), false);

  ImGui::StyleColorsDark();
#endif // BIG2_IMGUI_ENABLED

  NormalColorVertex kTriangleVertices[] =
      {
          {{-0.5f, -0.5f}, 0x339933FF},
          {{0.5f, -0.5f}, 0x993333FF},
          {{0.0f, 0.5f}, 0x333399FF},
      };

  const uint16_t kTriangleIndices[] =
      {
          0, 1, 2,
      };

  bgfx::VertexLayout color_vertex_layout;
  color_vertex_layout.begin()
      .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
      .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
      .end();

  bgfx::VertexBufferHandle vertex_buffer = bgfx::createVertexBuffer(bgfx::makeRef(kTriangleVertices, sizeof(kTriangleVertices)), color_vertex_layout);
  bgfx::IndexBufferHandle index_buffer = bgfx::createIndexBuffer(bgfx::makeRef(kTriangleIndices, sizeof(kTriangleIndices)));

  gsl::final_action destroy_buffers([&vertex_buffer, &index_buffer]() {
    bgfx::destroy(index_buffer);
    bgfx::destroy(vertex_buffer);
  });

  bgfx::RendererType::Enum renderer_type = bgfx::getRendererType();
  bgfx::ProgramHandle program = bgfx::createProgram(
      bgfx::createEmbeddedShader(kEmbeddedShaders, renderer_type, "vs_basic"),
      bgfx::createEmbeddedShader(kEmbeddedShaders, renderer_type, "fs_basic"),
      true
  );

  gsl::final_action destroy_program([&program]() { bgfx::destroy(program); });

  big2::GlfwEventQueue::Initialize();
  big2::GlfwEventQueue::ConnectWindow(window);
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

    bgfx::setState(
        BGFX_STATE_WRITE_R
            | BGFX_STATE_WRITE_G
            | BGFX_STATE_WRITE_B
            | BGFX_STATE_WRITE_A
    );

    bgfx::setVertexBuffer(0, vertex_buffer);
    bgfx::setIndexBuffer(index_buffer);
    bgfx::submit(window.GetView(), program);

    bgfx::frame();
  }

  return 0;
}
