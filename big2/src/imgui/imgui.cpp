//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#include <big2/imgui/imgui.h>

#if BIG2_IMGUI_ENABLED

#include <backends/imgui_impl_glfw.h>
#include <big2/imgui/imgui_impl_bgfx.h>

namespace big2 {

void ImGuiInit(GLFWwindow *window, bgfx::ViewId view_id) {
  ImGui_ImplGlfw_InitForOther(window, true);
  ImGui_ImplBgfx_Init(view_id);
}

void ImGuiTerminate() {
  ImGui_ImplBgfx_Shutdown();
  ImGui_ImplGlfw_Shutdown();
}

void ImGuiBeginFrame() {
  ImGui_ImplBgfx_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void ImGuiEndFrame() {
  ImGui::EndFrame();
  ImGui::Render();
  ImGui_ImplBgfx_RenderDrawData(ImGui::GetDrawData());
}

}

#endif //BIG2_IMGUI_ENABLED
