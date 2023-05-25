//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#include <big2/imgui/imgui.h>

#if BIG2_IMGUI_ENABLED

#include <backends/imgui_impl_glfw.h>
#include <big2/imgui/imgui_impl_bgfx.h>

namespace big2 {

ImGuiContext * ImGuiInit(GLFWwindow *window, bgfx::ViewId view_id, bool use_default_callbacks) {
  IMGUI_CHECKVERSION();

  ImGuiContext* context = ImGui::CreateContext();
  ImGui::SetCurrentContext(context);

  ImGui_ImplGlfw_InitForOther(window, use_default_callbacks);
  ImGui_ImplBgfx_Init(view_id);
  return context;
}

void ImGuiTerminate(ImGuiContext *context) {
  ImGuiContext *previous_context = ImGui::GetCurrentContext();
  if (context == nullptr) {
    context = previous_context;
  }

  ImGui::SetCurrentContext(context);
  ImGui_ImplBgfx_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext(context);

  if (context != previous_context) {
    ImGui::SetCurrentContext(previous_context);
  }
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
