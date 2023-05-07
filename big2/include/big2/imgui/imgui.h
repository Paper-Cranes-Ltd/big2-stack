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
void ImGuiInit(GLFWwindow *window, bgfx::ViewId view_id);
void ImGuiTerminate();
void ImGuiBeginFrame();
void ImGuiEndFrame();
}
#endif // BIG2_IMGUI_ENABLED

#endif //BIG2_STACK_BIG2_IMGUI_IMGUI_H_
