//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//
#include <big2/imgui/imgui_app_extension.h>

#if BIG2_IMGUI_ENABLED

#include <big2/algorithm.h>
#include <big2/event_queue.h>
#include <big2/app.h>
#include <gsl/narrow>
#include <big2/macros.h>
#include <big2/imgui/imgui_frame_scoped.h>

namespace big2 {

void ImGuiAppExtension::OnWindowCreated(Window& window) {
  AppExtensionBase::OnWindowCreated(window);
  ImGuiContextWrapper context;
  context.SetIsScoped(false);

  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

#if defined(IMGUI_HAS_DOCK)
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable docking
#endif // defined(IMGUI_HAS_DOCK)

#if defined(IMGUI_HAS_VIEWPORT)
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // Enable viewports
#endif // defined(IMGUI_HAS_VIEWPORT)

  context.Initialize(window, window.GetView(), /* use_default_callbacks= */ false);

  ImGui::StyleColorsDark();

  contexts_.push_back(context);
}
void ImGuiAppExtension::OnWindowDestroyed(Window& window) {
  AppExtensionBase::OnWindowDestroyed(window);
  std::optional<ImGuiContextWrapper> maybe_context = big2::FirstIf<ImGuiContextWrapper>(contexts_.begin(), contexts_.end(), [&window](ImGuiContextWrapper& context) {
    return context.GetWindow() == window.GetWindowHandle();
  });

  if(maybe_context.has_value())
  {
    maybe_context.value().Dispose();
  }

  std::erase_if(contexts_,
                [window](ImGuiContextWrapper &context) {
                  return context.GetWindow() == window.GetWindowHandle();
                });
}

void ImGuiAppExtension::OnWindowUpdate(Window& window) {
  AppExtensionBase::OnWindowUpdate(window);

  std::optional<ImGuiContextWrapper> maybe_context = big2::FirstIf<ImGuiContextWrapper>(contexts_.begin(), contexts_.end(), [&window](ImGuiContextWrapper& context) {
    return context.GetWindow() == window.GetWindowHandle();
  });

  if(!maybe_context.has_value())
  {
    return;
  }

  ImGui::SetCurrentContext(maybe_context.value().GetContext());
  big2::GlfwEventQueue::UpdateImGuiEvents(window.GetWindowHandle());

  BIG2_SCOPE_VAR(big2::ImGuiFrameScoped) {
    ImGui::ShowDemoWindow();
  }
}

}

#endif // BIG2_IMGUI_ENABLED


