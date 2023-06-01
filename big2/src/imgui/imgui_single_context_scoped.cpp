//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//
#include <big2/imgui/imgui_single_context_scoped.h>
#include <big2/asserts.h>
#include <big2/imgui/imgui.h>

namespace big2 {

ImGuiSingleContextScoped::ImGuiSingleContextScoped() {
  big2::Validate(ImGui::GetCurrentContext() == nullptr, "ImGuiSingleContextScoped should only be used in a single context scenario.");
  context_ = ImGui::CreateContext();
  ImGui::SetCurrentContext(context_);
}

ImGuiSingleContextScoped::~ImGuiSingleContextScoped() {
  big2::SoftValidate(ImGui::GetCurrentContext() == context_, "ImGuiSingleContextScoped should only be used in a single context scenario. Another context detected on termination.");
  big2::ImGuiTerminate(context_);
}

void ImGuiSingleContextScoped::Initialize(gsl::not_null<GLFWwindow *> window, bgfx::ViewId view_id, bool use_default_callbacks) {
  big2::ImGuiInit(context_, window, view_id, use_default_callbacks);
}

}
