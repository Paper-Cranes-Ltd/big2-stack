//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_IMGUI_IMGUI_H_
#define BIG2_STACK_IMGUI_IMGUI_H_

#if BIG2_IMGUI_ENABLED
#include <bgfx/bgfx.h>
#include <imgui_internal.h>
#include <gsl/gsl>

struct GLFWwindow;

namespace big2 {

/**
 * @brief Initializes ImGui with the official backend of glfw and a big2 renderer
 * @details Will create and set as current a new ImGuiContext.
 * Be careful if you're using multi-context setup that you keep a reference to your previous context
 * @param window An initialized window handle
 * @param view_id A bgfx view id on which imgui will draw to
 * @param use_default_callbacks Whether to let imgui install glfw input event callbacks.
 * They don't work with multiple ImGui contexts so I recommend using the GlfwEventQueue.
 */
ImGuiContext *ImGuiInit(gsl::not_null<GLFWwindow *> window, bgfx::ViewId view_id, bool use_default_callbacks = false);

/**
 * @brief Terminates the ImGui renderer and backend
 * @details If this context is not passed it will terminate the current context.
 * If a context is passed that is different than the current one, then the current one will be restored after the passed one is terminated.
 * @param context The context that you want to terminate.
 */
void ImGuiTerminate(ImGuiContext *context = nullptr);

/**
 * @brief Calls out a new frame for the renderer, backend and ImGui
 */
void ImGuiBeginFrame();

/**
 * @brief Finalizes the frame draw data ending the ImGui frame and rendering it
 */
void ImGuiEndFrame();

/**
 * @brief Will call ImGuiStartFrame() and ImGuiEndFrame() in a scope
 * @details Could be used with some helper macros in this project.
 * @see BIG2_SCOPE(assignment)
 */
class ImGuiFrameScoped final {
 public:
  ImGuiFrameScoped();
  ~ImGuiFrameScoped();
};

/**
 * @brief Helps when having a single-window single-context scenario.
 * @details Will handle creation and disposal of the single ImGuiContext pointer
 * as well as initializing GLFW and BGFX with ImGui.
 * @see BIG2_SCOPE(assignment)
 */
class ImGuiSingleContextScoped final {
 public:
  ImGuiSingleContextScoped(gsl::not_null<GLFWwindow *> window, bgfx::ViewId view_id, bool use_default_callbacks = true);
  ImGuiSingleContextScoped(ImGuiSingleContextScoped&&) = default;
  ImGuiSingleContextScoped& operator=(ImGuiSingleContextScoped&&) = default;
  ImGuiSingleContextScoped(const ImGuiSingleContextScoped&) = delete;
  ImGuiSingleContextScoped& operator=(const ImGuiSingleContextScoped&) = delete;
  ~ImGuiSingleContextScoped();
 private:
  ImGuiContext *context_ = nullptr;
};

}
#endif // BIG2_IMGUI_ENABLED

#endif //BIG2_STACK_IMGUI_IMGUI_H_
