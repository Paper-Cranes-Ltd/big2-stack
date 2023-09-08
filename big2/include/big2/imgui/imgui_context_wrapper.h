//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_IMGUI_CONTEXT_WRAPPER_H_
#define BIG2_STACK_IMGUI_CONTEXT_WRAPPER_H_

#include <big2/imgui/imgui.h>

struct GLFWwindow;

namespace big2 {

/**
 * @brief Helps when having a single-window single-context scenario.
 * @details Will handle creation and disposal of the single ImGuiContext pointer
 * as well as initializing GLFW and BGFX with ImGui.
 * @see BIG2_SCOPE(assignment)
 */
class ImGuiContextWrapper final {
 public:
  ImGuiContextWrapper();
  ImGuiContextWrapper(ImGuiContextWrapper &&) = default;
  ImGuiContextWrapper &operator=(ImGuiContextWrapper &&) = default;
  ImGuiContextWrapper(const ImGuiContextWrapper &) = default;
  ImGuiContextWrapper &operator=(const ImGuiContextWrapper &) = default;
  ~ImGuiContextWrapper();

  void Dispose();
  void Initialize(gsl::not_null<GLFWwindow *> window, bgfx::ViewId view_id, bool use_default_callbacks = true);
  [[nodiscard]] ImGuiContext *GetContext() const { return context_; }
  [[nodiscard]] GLFWwindow *GetWindow() const { return window_; }
  [[nodiscard]] bool GetIsScoped() const { return is_scoped_; }
  ImGuiContextWrapper& SetIsScoped(bool scoped);

 private:
  ImGuiContext *context_ = nullptr;
  GLFWwindow *window_ = nullptr;
  bool is_scoped_ = true;
};

}

#endif //BIG2_STACK_IMGUI_CONTEXT_WRAPPER_H_
