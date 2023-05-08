//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_BIG2_IMGUI_IMGUI_H_
#define BIG2_STACK_BIG2_IMGUI_IMGUI_H_

#if BIG2_IMGUI_ENABLED
#include <bgfx/bgfx.h>

struct GLFWwindow;

namespace big2
{

/**
 * @brief Initializes ImGui with the official backend of glfw and a big2 renderer
 * @param window An initialized window handle
 * @param view_id A bgfx view id on which imgui will draw to
 */
void ImGuiInit(GLFWwindow *window, bgfx::ViewId view_id);

/**
 * @brief Terminates the ImGui renderer and backend
 */
void ImGuiTerminate();

/**
 * @brief Calls out a new frame for the renderer, backend and ImGui
 */
void ImGuiBeginFrame();

/**
 * @brief Finalizes the frame draw data ending the ImGui frame and rendering it
 */
void ImGuiEndFrame();
}
#endif // BIG2_IMGUI_ENABLED

#endif //BIG2_STACK_BIG2_IMGUI_IMGUI_H_
