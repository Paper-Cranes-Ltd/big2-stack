//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

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
#include <big2/bgfx/bgfx_utils.h>
#include <big2/glfw/glfw_utils.h>
#include <big2/void_ptr.h>

constexpr const std::uint64_t kState =
    BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);

struct BackendRendererData {
  bgfx::ViewId view_id = 0;
  bgfx::TextureHandle font_texture_handle = BGFX_INVALID_HANDLE;
  bgfx::ProgramHandle shader_handle = BGFX_INVALID_HANDLE;
  bgfx::UniformHandle texture_location_handle = BGFX_INVALID_HANDLE;
  bgfx::VertexLayout vertex_layout;
};

struct Rgba32TextureData {
  std::uint8_t *bytes;
  glm::u16vec2 size;
  std::int32_t bytes_per_pixel;

  [[nodiscard]] std::size_t GetBytesCount() const { return size.x * size.y * bytes_per_pixel; }
};

static gsl::not_null<BackendRendererData *> ImGui_ImplBgfx_GetBackendData() {
  big2::Validate(ImGui::GetCurrentContext() != nullptr, "Current context must be valid");
  big2::VoidPtr raw_data = ImGui::GetIO().BackendRendererUserData;
  big2::Validate(raw_data.IsValid(), "Backend renderer data is invalid!");
  return raw_data.RCast<BackendRendererData>();
}

#include <generated/shaders/big2/all.h>

static const bgfx::EmbeddedShader EmbeddedShaders[] =
    {
        BGFX_EMBEDDED_SHADER(vs_ocornut_imgui),
        BGFX_EMBEDDED_SHADER(fs_ocornut_imgui),
        BGFX_EMBEDDED_SHADER_END()
    };

void ExecuteRenderCommands(const ImDrawData *draw_data, unsigned short view_id, bgfx::ProgramHandle program, bgfx::UniformHandle texture_location, const bgfx::VertexLayout layout, const glm::vec2 frame_location, glm::vec2 frame_size) {
  const bgfx::Caps *caps = bgfx::getCaps();

  float_t orthographic_view[16];
  bx::mtxOrtho(orthographic_view, frame_location.x, frame_location.x + frame_size.x, frame_location.y + frame_size.y, frame_location.y, 0.0f, 1000.0f, 0.0f, caps->homogeneousDepth);
  bgfx::setViewTransform(view_id, nullptr, orthographic_view);
  bgfx::setViewRect(view_id, 0, 0, gsl::narrow_cast<uint16_t>(frame_size.x), gsl::narrow_cast<uint16_t>(frame_size.y));

  // Render command lists
  for (int n = 0; n < draw_data->CmdListsCount; n++) {
    const ImDrawList *command_list = draw_data->CmdLists[n];

    uint32_t requested_vertices_count = static_cast<uint32_t>(command_list->VtxBuffer.size());
    uint32_t requested_indices_count = static_cast<uint32_t>(command_list->IdxBuffer.size());

    bool has_vb_space = big2::SoftValidate(bgfx::getAvailTransientVertexBuffer(requested_vertices_count, layout) == requested_vertices_count, "Not enough space in vertex transient buffer.");
    bool has_ib_space = big2::SoftValidate(bgfx::getAvailTransientIndexBuffer(requested_indices_count) == requested_indices_count, "Not enough space in index transient buffer.");

    if (!has_vb_space || !has_ib_space) {
      break;
    }

    bgfx::TransientVertexBuffer transient_vtx_buffer{};
    bgfx::allocTransientVertexBuffer(&transient_vtx_buffer, requested_vertices_count, layout);
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
        const glm::u16vec2 position(bx::max(draw_command->ClipRect.x - frame_location.x, 0.0f), bx::max(draw_command->ClipRect.y - frame_location.y, 0.0f));
        const glm::u16vec2 end_position(bx::min(draw_command->ClipRect.z - frame_location.x, 65535.0f), bx::min(draw_command->ClipRect.w - frame_location.y, 65535.0f));
        const glm::u16vec2 size = end_position - position;
        bgfx::setScissor(position.x, position.y, size.x, size.y);

        bgfx::setState(kState);

        bgfx::TextureHandle texture = {static_cast<uint16_t>(reinterpret_cast<intptr_t>(draw_command->TextureId) & 0xffff)};

        bgfx::setTexture(0, texture_location, texture);
        bgfx::setVertexBuffer(0, &transient_vtx_buffer, 0, requested_vertices_count);
        bgfx::setIndexBuffer(&transient_idx_buffer, draw_command->IdxOffset, draw_command->ElemCount);
        bgfx::submit(view_id, program);
      }
    }
  }
}

#pragma region Viewports

#if defined(IMGUI_HAS_VIEWPORT)

struct BackendRendererViewportData {
  BackendRendererViewportData(gsl::not_null<GLFWwindow *> window) : frame_buffer(window) {
  }

  big2::BgfxViewScoped view;
  big2::BgfxFrameBufferScoped frame_buffer;
};

void ViewportCreateWindow(ImGuiViewport *viewport) {
  GLFWwindow *window = reinterpret_cast<GLFWwindow *>(viewport->PlatformHandle);
  glm::ivec2 resolution = big2::GetWindowResolution(window);

  BackendRendererViewportData *data = IM_NEW(BackendRendererViewportData)(window);
  viewport->RendererUserData = data;

  bgfx::setViewFrameBuffer(data->view, data->frame_buffer);
  bgfx::setViewRect(data->view, 0, 0, resolution.x, resolution.y);
}

void ViewportDestroyWindow(ImGuiViewport *viewport) {
  big2::VoidPtr data = viewport->RendererUserData;
  if (data.IsValid()) {
    IM_DELETE<void>(data);
  }

  viewport->RendererUserData = nullptr;
}

void ViewportSetWindowSize(ImGuiViewport *viewport, ImVec2 size) {
  GLFWwindow *window = reinterpret_cast<GLFWwindow *>(viewport->PlatformHandle);
  glm::ivec2 resolution = big2::GetWindowResolution(window);

  big2::VoidPtr raw_data = viewport->RendererUserData;
  big2::Validate(raw_data.IsValid(), "The viewport renderer wasn't initialized correctly!");
  BackendRendererViewportData *data = raw_data.RCast<BackendRendererViewportData>();
  big2::ResetWindowFrameBuffer(window, data->frame_buffer);

  bgfx::setViewFrameBuffer(data->view, data->frame_buffer);
  bgfx::setViewRect(data->view, 0, 0, resolution.x, resolution.y);
}

void ViewportRenderWindow(ImGuiViewport *viewport, auto) {

  big2::VoidPtr raw_data = viewport->RendererUserData;
  big2::Validate(raw_data.IsValid(), "The viewport renderer wasn't initialized correctly!");
  BackendRendererViewportData *data = raw_data.RCast<BackendRendererViewportData>();

  if (viewport->Flags & ImGuiViewportFlags_NoRendererClear)
  {
    bgfx::setViewClear(data->view, BGFX_CLEAR_DEPTH);
  }
  else
  {
    bgfx::setViewClear(data->view, BGFX_CLEAR_DEPTH | BGFX_CLEAR_COLOR);
  }

  ImGuiIO &io = ImGui::GetIO();
  BackendRendererData *backend_data = ImGui_ImplBgfx_GetBackendData();
  bgfx::ProgramHandle program = backend_data->shader_handle;
  bgfx::UniformHandle texture_location = backend_data->texture_location_handle;
  bgfx::VertexLayout layout = backend_data->vertex_layout;
  const glm::vec2 frame_location(viewport->DrawData->DisplayPos.x, viewport->DrawData->DisplayPos.y);
  const glm::vec2 frame_size(viewport->DrawData->DisplaySize.x * viewport->DrawData->FramebufferScale.x, viewport->DrawData->DisplaySize.y * viewport->DrawData->FramebufferScale.y);

  if (frame_size.x == 0 || frame_size.y == 0) {
    return;
  }

  viewport->DrawData->ScaleClipRects(io.DisplayFramebufferScale);

  ExecuteRenderCommands(viewport->DrawData, data->view, program, texture_location, layout, frame_location, frame_size);
}

void InitializeViewportInterface() {
  ImGuiPlatformIO &platform_io = ImGui::GetPlatformIO();
  platform_io.Renderer_RenderWindow = ViewportRenderWindow;
  platform_io.Renderer_CreateWindow = ViewportCreateWindow;
  platform_io.Renderer_DestroyWindow = ViewportDestroyWindow;
  platform_io.Renderer_SetWindowSize = ViewportSetWindowSize;
}

void ShutdownViewportInterface() {
  ImGui::DestroyPlatformWindows();
}

#endif // defined(IMGUI_HAS_VIEWPORT)

#pragma endregion

void InitializeImGuiIO() {
  ImGuiIO &io = ImGui::GetIO();
  big2::Validate(io.BackendRendererUserData == nullptr, "Already initialized a renderer backend!");
  io.BackendRendererUserData = big2::VoidPtr(IM_NEW(BackendRendererData)());
  io.BackendRendererName = "imgui_impl_bgfx";

#if defined(IMGUI_HAS_VIEWPORT)
  io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    InitializeViewportInterface();
  }
#endif // defined(IMGUI_HAS_VIEWPORT)

}

bool ImGui_ImplBgfx_Init(bgfx::ViewId view_id) {
  InitializeImGuiIO();
  gsl::not_null<BackendRendererData *> backend_data = ImGui_ImplBgfx_GetBackendData();
  backend_data->view_id = view_id;
  return true;
}

void ImGui_ImplBgfx_Shutdown() {
  BackendRendererData *backend_data = ImGui_ImplBgfx_GetBackendData();
  big2::Validate(backend_data != nullptr, "No backend data (or renderer already shutdown)");
  ImGuiIO &io = ImGui::GetIO();

#if defined(IMGUI_HAS_VIEWPORT)
  ShutdownViewportInterface();
#endif // defined(IMGUI_HAS_VIEWPORT)

  ImGui_ImplBgfx_DestroyDeviceObjects();
  io.BackendRendererName = nullptr;
  io.BackendRendererUserData = nullptr;
#if defined(IMGUI_HAS_VIEWPORT)
  io.BackendFlags &= ~(ImGuiBackendFlags_RendererHasVtxOffset | ImGuiBackendFlags_RendererHasViewports);
#else
  io.BackendFlags &= ~(ImGuiBackendFlags_RendererHasVtxOffset);
#endif // defined(IMGUI_HAS_VIEWPORT)
  IM_DELETE(backend_data);
}

void ImGui_ImplBgfx_NewFrame() {
  gsl::not_null<BackendRendererData *> backend_data = ImGui_ImplBgfx_GetBackendData();

  if (!isValid(backend_data->font_texture_handle)) {
    ImGui_ImplBgfx_CreateDeviceObjects();
  }
}

void ImGui_ImplBgfx_RenderDrawData(ImDrawData *draw_data) {
  gsl::not_null<BackendRendererData *> backend_data = ImGui_ImplBgfx_GetBackendData();
  bgfx::ViewId view_id = backend_data->view_id;
  bgfx::ProgramHandle program = backend_data->shader_handle;
  bgfx::UniformHandle texture_location = backend_data->texture_location_handle;
  bgfx::VertexLayout layout = backend_data->vertex_layout;

  // Avoid rendering when minimized, scale coordinates for retina displays
  // (screen coordinates != framebuffer coordinates)
  ImGuiIO &io = ImGui::GetIO();

#if defined(IMGUI_HAS_VIEWPORT)
  const glm::vec2 frame_location(draw_data->DisplayPos.x, draw_data->DisplayPos.y);
  const glm::vec2 frame_size(draw_data->DisplaySize.x * draw_data->FramebufferScale.x, draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
#else
  glm::vec2 frame_location(0, 0);
  glm::vec2 frame_size(io.DisplaySize.x * io.DisplayFramebufferScale.x, io.DisplaySize.y * io.DisplayFramebufferScale.y);
#endif

  if (frame_size.x == 0 || frame_size.y == 0) {
    return;
  }

  draw_data->ScaleClipRects(io.DisplayFramebufferScale);

  ExecuteRenderCommands(draw_data, view_id, program, texture_location, layout, frame_location, frame_size);
}

Rgba32TextureData GetTextureData() {
  ImGuiIO &io = ImGui::GetIO();
  Rgba32TextureData result{};
  std::int32_t x, y;
  io.Fonts->GetTexDataAsRGBA32(&result.bytes, &x, &y, &result.bytes_per_pixel);
  result.size = {static_cast<std::uint16_t>(x), static_cast<std::uint16_t>(y)};
  Expects(result.bytes_per_pixel == 4); // Since it is RGBA32
  return result;
}

bool ImGui_ImplBgfx_CreateFontsTexture() {
  gsl::not_null<BackendRendererData *> backend_data = ImGui_ImplBgfx_GetBackendData();
  Rgba32TextureData texture_data = GetTextureData();

  constexpr bool kHasMips = false;
  constexpr std::uint16_t kLayersCount = 1;
  constexpr std::uint64_t kFlags = 0;

  const bgfx::Memory *data = bgfx::copy(texture_data.bytes, static_cast<std::uint32_t>(texture_data.GetBytesCount()));
  backend_data->font_texture_handle = bgfx::createTexture2D(texture_data.size.x, texture_data.size.y, kHasMips, kLayersCount, bgfx::TextureFormat::BGRA8, kFlags, data);

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
