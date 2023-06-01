//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_IMGUI_SINGLE_CONTEXT_SCOPED_H_
#define BIG2_STACK_IMGUI_SINGLE_CONTEXT_SCOPED_H_

#include <imgui.h>
#include <gsl/gsl>
#include <bgfx/bgfx.h>

struct GLFWwindow;

namespace big2 {

/**
 * @brief Helps when having a single-window single-context scenario.
 * @details Will handle creation and disposal of the single ImGuiContext pointer
 * as well as initializing GLFW and BGFX with ImGui.
 * @see BIG2_SCOPE(assignment)
 */
class ImGuiSingleContextScoped final {
 public:
  ImGuiSingleContextScoped();
  ImGuiSingleContextScoped(ImGuiSingleContextScoped&&) = default;
  ImGuiSingleContextScoped& operator=(ImGuiSingleContextScoped&&) = default;
  ImGuiSingleContextScoped(const ImGuiSingleContextScoped&) = delete;
  ImGuiSingleContextScoped& operator=(const ImGuiSingleContextScoped&) = delete;
  ~ImGuiSingleContextScoped();

  void Initialize(gsl::not_null<GLFWwindow *> window, bgfx::ViewId view_id, bool use_default_callbacks = true);
 private:
  ImGuiContext *context_ = nullptr;
};

}

#endif //BIG2_STACK_IMGUI_SINGLE_CONTEXT_SCOPED_H_
