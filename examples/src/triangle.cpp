//
// Copyright (c) 2023 ThatOneGameDev.com
// All rights reserved.
//

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include <big2.h>
#include <cstdint>

#include <glm/glm.hpp>
#include <big2/embedded_shader.h>
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

static void GlfwErrorCallback(int error, const char *description) {
  std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main(int, char **) {
  glfwSetErrorCallback(GlfwErrorCallback);
  Ensures(glfwInit());
  gsl::final_action terminate_glfw([]() { glfwTerminate(); });

  GLFWwindow *window = glfwCreateWindow(800, 600, "Hello, BGFX!", nullptr, nullptr);
  Ensures(window != nullptr);
  gsl::final_action destroy_window([window]() { glfwDestroyWindow(window); });

  // Inside main after window is created and before main loop
  bgfx::Init init_object;
  big2::SetNativeWindowData(init_object, window);

  glm::ivec2 window_size = big2::GetWindowSize(window);
  init_object.resolution.width = window_size.x;
  init_object.resolution.height = window_size.y;
  init_object.resolution.reset = BGFX_RESET_VSYNC;

  Ensures(bgfx::init(init_object));

  // We will use this to reference where we're drawing
  const bgfx::ViewId main_view_id = 0;

  // This is set once to determine the clear color to use on starting a new frame
  bgfx::setViewClear(main_view_id, BGFX_CLEAR_COLOR, 0x000000FF);

  // This is set to determine the size of the drawable surface
  bgfx::setViewRect(main_view_id, 0, 0, window_size.x, window_size.y);

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

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    // Inside the main loop after glfwPollEvents
    // Check if window size changed and update the view respectively
    const glm::ivec2 new_window_size = big2::GetWindowSize(window);
    if (new_window_size != window_size) {
      bgfx::reset(new_window_size.x, new_window_size.y, BGFX_RESET_VSYNC);
      bgfx::setViewRect(main_view_id, 0, 0, bgfx::BackbufferRatio::Equal);
      window_size = new_window_size;
    }

    // Ensure the view is redrawn even if no graphic commands are called
    bgfx::touch(main_view_id);

    bgfx::setState(
        BGFX_STATE_WRITE_R
            | BGFX_STATE_WRITE_G
            | BGFX_STATE_WRITE_B
            | BGFX_STATE_WRITE_A
    );

    bgfx::setVertexBuffer(0, vertex_buffer);
    bgfx::setIndexBuffer(index_buffer);
    bgfx::submit(main_view_id, program);

    // End the frame
    bgfx::frame();
  }
}
