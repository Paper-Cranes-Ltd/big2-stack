//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//
#include <big2/imgui/imgui_context_wrapper.h>
#include <big2/asserts.h>
#include <big2/imgui/imgui.h>

namespace big2 {

ImGuiContextWrapper::ImGuiContextWrapper() {
  context_ = ImGui::CreateContext();
  ImGui::SetCurrentContext(context_);
}

ImGuiContextWrapper::~ImGuiContextWrapper() {
  if (is_scoped_) {
    Dispose();
  }
}

void ImGuiContextWrapper::Initialize(gsl::not_null<GLFWwindow *> window, bgfx::ViewId view_id, bool use_default_callbacks) {
  window_ = window;
  big2::ImGuiInit(context_, window, view_id, use_default_callbacks);
}

ImGuiContextWrapper &ImGuiContextWrapper::SetIsScoped(bool scoped) {
  is_scoped_ = scoped;
  return *this;
}

void ImGuiContextWrapper::Dispose() {
  big2::ImGuiTerminate(context_);
}

}
