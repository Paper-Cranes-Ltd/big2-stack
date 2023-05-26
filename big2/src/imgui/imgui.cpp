//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#include <big2/imgui/imgui.h>

#if BIG2_IMGUI_ENABLED

#include <backends/imgui_impl_glfw.h>
#include <big2/imgui/imgui_impl_bgfx.h>
#include <big2/asserts.h>
#include <big2/macros.h>

namespace big2 {

ImGuiContext * ImGuiInit(gsl::not_null<GLFWwindow *> window, bgfx::ViewId view_id, bool use_default_callbacks) {
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

ImGuiFrameScoped::ImGuiFrameScoped() {
  big2::ImGuiBeginFrame();
}

ImGuiFrameScoped::~ImGuiFrameScoped() {
  big2::ImGuiEndFrame();
}

ImGuiSingleContextScoped::ImGuiSingleContextScoped(gsl::not_null<GLFWwindow *> window, bgfx::ViewId view_id, bool use_default_callbacks) {
  big2::Validate(ImGui::GetCurrentContext() == nullptr, "ImGuiSingleContextScoped should only be used in a single context scenario.");
  context_ = big2::ImGuiInit(window, view_id, use_default_callbacks);
}

ImGuiSingleContextScoped::~ImGuiSingleContextScoped() {
  big2::SoftValidate(ImGui::GetCurrentContext() == context_, "ImGuiSingleContextScoped should only be used in a single context scenario. Another context detected on termination.");
  big2::ImGuiTerminate(context_);
}

}

#endif //BIG2_IMGUI_ENABLED
