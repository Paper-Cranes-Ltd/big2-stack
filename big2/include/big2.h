//
// Copyright (c) 2023 ThatOneGameDev.com
// All rights reserved.
//

#ifndef BIG2_STACK_BIG2_INCLUDE_BIG2_H_
#define BIG2_STACK_BIG2_INCLUDE_BIG2_H_

#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <gsl/span>
struct GLFWwindow;
struct GLFWmonitor;

namespace big2
{

#if BIG2_IMGUI_ENABLED
void ImGuiInit(GLFWwindow *window, bgfx::ViewId view_id);
void ImGuiTerminate();
void ImGuiBeginFrame();
void ImGuiEndFrame();
#endif // BIG2_IMGUI_ENABLED

void SetNativeWindowData(bgfx::Init &init_obj, GLFWwindow *window);
gsl::span<GLFWmonitor*> GetMonitors();

}

#endif //BIG2_STACK_BIG2_INCLUDE_BIG2_H_
