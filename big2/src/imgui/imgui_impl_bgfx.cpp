//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//
// dear imgui: Renderer Backend for BGFX with shaders / programmatic pipeline
// This needs to be used along with a Platform Backend (e.g. GLFW, SDL, Win32, custom...)

// Implemented features:
//  [X] Renderer: User texture binding. Use 'bgfx::TextureHandle' library texture identifier as void*/ImTextureID. Read the FAQ about ImTextureID!

// You can use unmodified imgui_impl_* files in your project. See examples/ folder for examples of using this.
// Prefer including the entire imgui/ repository into your project (either as a copy or as a submodule), and only build the backends you need.
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

// CHANGELOG
// 2023-03-23: Renderer: Added the main code that displays imgui.

#include <imgui.h>
#include <big2/imgui/imgui_impl_bgfx.h>

#include <cstdint> // sized integers
#include <bgfx/bgfx.h>
#include <big2/bgfx/embedded_shader.h>
#include <bx/math.h>
#include <cmath>
#include <algorithm>
#include <gsl/gsl>
#include <big2/asserts.h>
#include <big2/macros.h>
#include <glm/glm.hpp>

struct BackendRendererData {
  bgfx::ViewId view_id = 0;
  bgfx::TextureHandle font_texture_handle = BGFX_INVALID_HANDLE;
  bgfx::ProgramHandle shader_handle = BGFX_INVALID_HANDLE;
  bgfx::UniformHandle texture_location_handle = BGFX_INVALID_HANDLE;
  bgfx::VertexLayout vertex_layout;
};

struct Rgba32TextureData {
  std::uint8_t *bytes;
  glm::ivec2 size;
  std::int32_t bytes_per_pixel;

  [[nodiscard]] std::size_t GetBytesCount() const { return size.x * size.y * bytes_per_pixel; }
};

static gsl::not_null<BackendRendererData *> ImGui_ImplBgfx_GetBackendData() {
  big2::Validate(ImGui::GetCurrentContext() != nullptr, "Current context must be valid");
  return reinterpret_cast<BackendRendererData *>(ImGui::GetIO().BackendRendererUserData);
}

#include <generated/shaders/big2/all.h>

static const bgfx::EmbeddedShader EmbeddedShaders[] =
    {
        BGFX_EMBEDDED_SHADER(vs_ocornut_imgui),
        BGFX_EMBEDDED_SHADER(fs_ocornut_imgui),
        BGFX_EMBEDDED_SHADER_END()
    };

void InitializeImGuiIO() {
  ImGuiIO &io = ImGui::GetIO();
  big2::Validate(io.BackendRendererUserData == nullptr, "Already initialized a renderer backend!");
  io.BackendRendererUserData = reinterpret_cast<void *>(IM_NEW(BackendRendererData)());
  io.BackendRendererName = "imgui_impl_bgfx";
}

bool ImGui_ImplBgfx_Init(bgfx::ViewId view_id) {
  InitializeImGuiIO();
  gsl::not_null<BackendRendererData *> backend_data = ImGui_ImplBgfx_GetBackendData();
  backend_data->view_id = view_id;
  return true;
}

void ImGui_ImplBgfx_Shutdown() {
  ImGui_ImplBgfx_DestroyDeviceObjects();
}

void ImGui_ImplBgfx_NewFrame() {
  gsl::not_null<BackendRendererData *> backend_data = ImGui_ImplBgfx_GetBackendData();

  if (!isValid(backend_data->font_texture_handle)) {
    ImGui_ImplBgfx_CreateDeviceObjects();
  }
}

void ImGui_ImplBgfx_RenderDrawData(ImDrawData *draw_data) {
  gsl::not_null<BackendRendererData *> backend_data = ImGui_ImplBgfx_GetBackendData();

  // Avoid rendering when minimized, scale coordinates for retina displays
  // (screen coordinates != framebuffer coordinates)
  ImGuiIO &io = ImGui::GetIO();
  glm::ivec2 frame_size(io.DisplaySize.x * io.DisplayFramebufferScale.x, io.DisplaySize.y * io.DisplayFramebufferScale.y);

  if (frame_size.x == 0 || frame_size.y == 0) {
    return;
  }

  draw_data->ScaleClipRects(io.DisplayFramebufferScale);

  // Setup render state: alpha-blending enabled, no face culling,
  // no depth testing, scissor enabled
  constexpr const std::uint64_t kState =
      BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);

  const bgfx::Caps *caps = bgfx::getCaps();

  std::float_t orthographic_view[16];
  bx::mtxOrtho(orthographic_view, 0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, 0.0f, 1000.0f, 0.0f, caps->homogeneousDepth);
  bgfx::setViewTransform(backend_data->view_id, nullptr, orthographic_view);
  bgfx::setViewRect(backend_data->view_id, 0, 0, frame_size.x, frame_size.y);

  // Render command lists
  for (int n = 0; n < draw_data->CmdListsCount; n++) {
    const ImDrawList *command_list = draw_data->CmdLists[n];

    std::uint32_t requested_vertices_count = static_cast<std::uint32_t>(command_list->VtxBuffer.size());
    std::uint32_t requested_indices_count = static_cast<std::uint32_t>(command_list->IdxBuffer.size());

    bool has_vb_space = big2::SoftValidate(bgfx::getAvailTransientVertexBuffer(requested_vertices_count, backend_data->vertex_layout) == requested_vertices_count, "Not enough space in vertex transient buffer.");
    bool has_ib_space = big2::SoftValidate(bgfx::getAvailTransientIndexBuffer(requested_indices_count) == requested_indices_count, "Not enough space in index transient buffer.");

    if (!has_vb_space || !has_ib_space) {
      break;
    }

    bgfx::TransientVertexBuffer transient_vtx_buffer{};
    bgfx::allocTransientVertexBuffer(&transient_vtx_buffer, requested_vertices_count, backend_data->vertex_layout);
    bgfx::TransientIndexBuffer transient_idx_buffer{};
    bgfx::allocTransientIndexBuffer(&transient_idx_buffer, requested_indices_count);

    gsl::span<ImDrawVert> vertices(reinterpret_cast<ImDrawVert *>(transient_vtx_buffer.data), transient_vtx_buffer.size);
    std::copy(command_list->VtxBuffer.begin(), command_list->VtxBuffer.end(), vertices.begin());

    gsl::span<ImDrawIdx> indices(reinterpret_cast<ImDrawIdx *>(transient_idx_buffer.data), transient_idx_buffer.size);
    std::copy(command_list->IdxBuffer.begin(), command_list->IdxBuffer.end(), indices.begin());

    for (int cmd_i = 0; cmd_i < command_list->CmdBuffer.Size; cmd_i++) {
      const ImDrawCmd *draw_command = &command_list->CmdBuffer[cmd_i];

      if (draw_command->UserCallback) {
        draw_command->UserCallback(command_list, draw_command);
      } else {
        const glm::ivec2 position(bx::max(draw_command->ClipRect.x, 0.0f), bx::max(draw_command->ClipRect.y, 0.0f));
        const glm::ivec2 end_position(bx::min(draw_command->ClipRect.z, 65535.0f), bx::min(draw_command->ClipRect.w, 65535.0f));
        const glm::ivec2 size = end_position - position;
        bgfx::setScissor(position.x, position.y, size.x, size.y);

        bgfx::setState(kState);

        bgfx::TextureHandle texture = {static_cast<uint16_t>(reinterpret_cast<std::intptr_t>(draw_command->TextureId) & 0xffff)};

        bgfx::setTexture(0, backend_data->texture_location_handle, texture);
        bgfx::setVertexBuffer(0, &transient_vtx_buffer, 0, requested_vertices_count);
        bgfx::setIndexBuffer(&transient_idx_buffer, draw_command->IdxOffset, draw_command->ElemCount);
        bgfx::submit(backend_data->view_id, backend_data->shader_handle);
      }
    }
  }
}

Rgba32TextureData GetTextureData() {
  ImGuiIO &io = ImGui::GetIO();
  Rgba32TextureData result{};
  io.Fonts->GetTexDataAsRGBA32(&result.bytes, &result.size.x, &result.size.y, &result.bytes_per_pixel);
  Expects(result.bytes_per_pixel == 4); // Since it is RGBA32
  return result;
}

bool ImGui_ImplBgfx_CreateFontsTexture() {
  gsl::not_null<BackendRendererData *> backend_data = ImGui_ImplBgfx_GetBackendData();
  Rgba32TextureData texture_data = GetTextureData();

  constexpr bool kHasMips = false;
  constexpr std::uint16_t kLayersCount = 1;
  constexpr std::uint64_t kFlags = 0;

  const bgfx::Memory *data = bgfx::copy(texture_data.bytes, texture_data.GetBytesCount());
  backend_data->font_texture_handle
      = bgfx::createTexture2D(texture_data.size.x, texture_data.size.y, kHasMips, kLayersCount, bgfx::TextureFormat::BGRA8, kFlags, data);

  ImGuiIO &io = ImGui::GetIO();
  io.Fonts->SetTexID(reinterpret_cast<ImTextureID>(static_cast<std::intptr_t>(backend_data->font_texture_handle.idx)));

  return true;
}

void ImGui_ImplBgfx_DestroyFontsTexture() {
  gsl::not_null<BackendRendererData *> backend_data = ImGui_ImplBgfx_GetBackendData();

  if (isValid(backend_data->font_texture_handle)) {
    bgfx::destroy(backend_data->font_texture_handle);
    ImGui::GetIO().Fonts->SetTexID(nullptr);
    backend_data->font_texture_handle = BGFX_INVALID_HANDLE;
  }
}

bool ImGui_ImplBgfx_CreateDeviceObjects() {
  gsl::not_null<BackendRendererData *> backend_data = ImGui_ImplBgfx_GetBackendData();

  bgfx::RendererType::Enum type = bgfx::getRendererType();
  backend_data->shader_handle = bgfx::createProgram(
      bgfx::createEmbeddedShader(EmbeddedShaders, type, "vs_ocornut_imgui"),
      bgfx::createEmbeddedShader(EmbeddedShaders, type, "fs_ocornut_imgui"),
      true
  );

  backend_data->vertex_layout.begin()
              .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
              .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
              .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
              .end();

  backend_data->texture_location_handle = bgfx::createUniform("g_AttribLocationTex", bgfx::UniformType::Sampler);

  ImGui_ImplBgfx_CreateFontsTexture();

  return true;
}

void ImGui_ImplBgfx_DestroyDeviceObjects() {
  gsl::not_null<BackendRendererData *> backend_data = ImGui_ImplBgfx_GetBackendData();

  bgfx::destroy(backend_data->texture_location_handle);
  bgfx::destroy(backend_data->shader_handle);

  ImGui_ImplBgfx_DestroyFontsTexture();
}
