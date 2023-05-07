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
#include <cstring> // memcpy

#include <bgfx/bgfx.h>
#include <big2/embedded_shader.h>
#include <bx/math.h>

#if __cplusplus >= 202301L
#include <stdfloat>
#else
#include <cmath>
namespace std
{
using float32_t = float_t;
}
#endif

struct ImGui_ImplBgfx_Data
{
  bgfx::ViewId ViewId = 0;
  bgfx::TextureHandle FontTextureHandle = BGFX_INVALID_HANDLE;
  bgfx::ProgramHandle ShaderHandle = BGFX_INVALID_HANDLE;
  bgfx::UniformHandle TextureLocationHandle = BGFX_INVALID_HANDLE;
  bgfx::VertexLayout VertexLayout;
};

static ImGui_ImplBgfx_Data *ImGui_ImplBgfx_GetBackendData()
{
    return ImGui::GetCurrentContext() != nullptr ? reinterpret_cast<ImGui_ImplBgfx_Data *>(ImGui::GetIO().BackendRendererUserData) : nullptr;
}

#include <generated/shaders/big2/all.h>

static const bgfx::EmbeddedShader EmbeddedShaders[] =
        {
                BGFX_EMBEDDED_SHADER(vs_ocornut_imgui),
                BGFX_EMBEDDED_SHADER(fs_ocornut_imgui),
                BGFX_EMBEDDED_SHADER_END()
        };

bool ImGui_ImplBgfx_Init(bgfx::ViewId view_id)
{
    ImGuiIO &io = ImGui::GetIO();
    IM_ASSERT(io.BackendRendererUserData == nullptr && "Already initialized a renderer backend!");

    ImGui_ImplBgfx_Data *bd = IM_NEW(ImGui_ImplBgfx_Data)();
    io.BackendRendererUserData = reinterpret_cast<void *>(bd);
    io.BackendRendererName = "imgui_impl_bgfx";

    bd->ViewId = view_id;

    return true;
}

void ImGui_ImplBgfx_Shutdown()
{
    ImGui_ImplBgfx_DestroyDeviceObjects();
}

void ImGui_ImplBgfx_NewFrame()
{
    ImGui_ImplBgfx_Data *bd = ImGui_ImplBgfx_GetBackendData();
    IM_ASSERT(bd != nullptr && "Bgfx renderer backend data not initialized!");

    if (!isValid(bd->FontTextureHandle))
    {
        ImGui_ImplBgfx_CreateDeviceObjects();
    }
}

void ImGui_ImplBgfx_RenderDrawData(ImDrawData *draw_data)
{
    ImGui_ImplBgfx_Data *bd = ImGui_ImplBgfx_GetBackendData();
    IM_ASSERT(bd != nullptr && "Bgfx renderer backend data not initialized!");

    // Avoid rendering when minimized, scale coordinates for retina displays
    // (screen coordinates != framebuffer coordinates)
    ImGuiIO &io = ImGui::GetIO();
    std::uint16_t frame_width = static_cast<std::uint16_t>(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    std::uint16_t frame_height = static_cast<std::uint16_t>(io.DisplaySize.y * io.DisplayFramebufferScale.y);

    if (frame_width == 0 || frame_height == 0)
    {
        return;
    }

    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    // Setup render state: alpha-blending enabled, no face culling,
    // no depth testing, scissor enabled
    constexpr const std::uint64_t state =
            BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);

    const bgfx::Caps *caps = bgfx::getCaps();

    std::float32_t orthographic_view[16];
    bx::mtxOrtho(orthographic_view, 0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, 0.0f, 1000.0f, 0.0f, caps->homogeneousDepth);
    bgfx::setViewTransform(bd->ViewId, nullptr, orthographic_view);
    bgfx::setViewRect(bd->ViewId, 0, 0, frame_width, frame_height);

    // Render command lists
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList *cmd_list = draw_data->CmdLists[n];

        std::uint32_t num_vertices = static_cast<std::uint32_t>(cmd_list->VtxBuffer.size());
        std::uint32_t num_indices = static_cast<std::uint32_t>(cmd_list->IdxBuffer.size());

        bool has_vb_space = num_vertices == bgfx::getAvailTransientVertexBuffer(num_vertices, bd->VertexLayout);
        bool has_ib_space = num_indices == bgfx::getAvailTransientIndexBuffer(num_indices);

        if (!has_vb_space || !has_ib_space)
        {
            // not enough space in transient buffer, quit drawing the rest...
            break;
        }

        bgfx::TransientVertexBuffer tvb;
        bgfx::allocTransientVertexBuffer(&tvb, num_vertices, bd->VertexLayout);
        bgfx::TransientIndexBuffer tib;
        bgfx::allocTransientIndexBuffer(&tib, num_indices);

        ImDrawVert *vertices = reinterpret_cast<ImDrawVert *>(tvb.data);
        std::memcpy(vertices, cmd_list->VtxBuffer.begin(), num_vertices * sizeof(ImDrawVert));

        ImDrawIdx *indices = reinterpret_cast<ImDrawIdx *>(tib.data);
        std::memcpy(indices, cmd_list->IdxBuffer.begin(), num_indices * sizeof(ImDrawIdx));

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd *pcmd = &cmd_list->CmdBuffer[cmd_i];

            if (pcmd->UserCallback)
            {
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                const std::uint16_t x_position = static_cast<std::uint16_t>(bx::max(pcmd->ClipRect.x, 0.0f));
                const std::uint16_t y_position = static_cast<std::uint16_t>(bx::max(pcmd->ClipRect.y, 0.0f));
                const std::uint16_t width = static_cast<std::uint16_t>(bx::min(pcmd->ClipRect.z, 65535.0f)) - x_position;
                const std::uint16_t height = static_cast<std::uint16_t>(bx::min(pcmd->ClipRect.w, 65535.0f)) - y_position;
                bgfx::setScissor(x_position, y_position, width, height);

                bgfx::setState(state);

                bgfx::TextureHandle texture = {static_cast<uint16_t>(reinterpret_cast<std::intptr_t>(pcmd->TextureId) & 0xffff)};

                bgfx::setTexture(0, bd->TextureLocationHandle, texture);
                bgfx::setVertexBuffer(0, &tvb, 0, num_vertices);
                bgfx::setIndexBuffer(&tib, pcmd->IdxOffset, pcmd->ElemCount);
                bgfx::submit(bd->ViewId, bd->ShaderHandle);
            }
        }
    }
}

bool ImGui_ImplBgfx_CreateFontsTexture()
{
    ImGuiIO &io = ImGui::GetIO();
    unsigned char *pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    ImGui_ImplBgfx_Data *bd = ImGui_ImplBgfx_GetBackendData();
    IM_ASSERT(bd != nullptr && "Bgfx renderer backend data not initialized!");

    constexpr bool hasMips = false;
    constexpr std::uint16_t layers = 1;
    constexpr std::uint64_t flags = 0;

    bd->FontTextureHandle = bgfx::createTexture2D(
            static_cast<std::uint16_t>(width),
            static_cast<std::uint16_t>(height),
            hasMips,
            layers,
            bgfx::TextureFormat::BGRA8,
            flags,
            bgfx::copy(pixels, width * height * 4));

    io.Fonts->SetTexID(reinterpret_cast<ImTextureID>(static_cast<std::intptr_t>(bd->FontTextureHandle.idx)));

    return true;
}

void ImGui_ImplBgfx_DestroyFontsTexture()
{
    ImGui_ImplBgfx_Data *bd = ImGui_ImplBgfx_GetBackendData();
    IM_ASSERT(bd != nullptr && "Bgfx renderer backend data not initialized!");

    if (isValid(bd->FontTextureHandle))
    {
        bgfx::destroy(bd->FontTextureHandle);
        ImGui::GetIO().Fonts->SetTexID(nullptr);
        bd->FontTextureHandle = BGFX_INVALID_HANDLE;
    }
}

bool ImGui_ImplBgfx_CreateDeviceObjects()
{
    ImGui_ImplBgfx_Data *bd = ImGui_ImplBgfx_GetBackendData();
    IM_ASSERT(bd != nullptr && "Bgfx renderer backend data not initialized!");

    bgfx::RendererType::Enum type = bgfx::getRendererType();
    bd->ShaderHandle = bgfx::createProgram(
            bgfx::createEmbeddedShader(EmbeddedShaders, type, "vs_ocornut_imgui"),
            bgfx::createEmbeddedShader(EmbeddedShaders, type, "fs_ocornut_imgui"),
            true
    );

    bd->VertexLayout.begin()
      .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
      .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
      .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
      .end();

    bd->TextureLocationHandle = bgfx::createUniform("g_AttribLocationTex", bgfx::UniformType::Sampler);

    ImGui_ImplBgfx_CreateFontsTexture();

    return true;
}

void ImGui_ImplBgfx_DestroyDeviceObjects()
{
    ImGui_ImplBgfx_Data *bd = ImGui_ImplBgfx_GetBackendData();
    IM_ASSERT(bd != nullptr && "Bgfx renderer backend data not initialized!");

    bgfx::destroy(bd->TextureLocationHandle);
    bgfx::destroy(bd->ShaderHandle);

    ImGui_ImplBgfx_DestroyFontsTexture();
}
