//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_IMGUI_IMGUI_H_
#define BIG2_STACK_IMGUI_IMGUI_H_

#if BIG2_IMGUI_ENABLED
#include <glm/glm.hpp>

#define IM_VEC2_CLASS_EXTRA                                                                   \
  constexpr ImVec2(glm::vec2 value) : x(value.x), y(value.y) {}                               \
  constexpr ImVec2(glm::vec3 value) : x(value.x), y(value.y) {}                               \
  constexpr ImVec2(glm::vec4 value) : x(value.x), y(value.y) {}                               \
  explicit(false) operator glm::vec2() const { return {x, y}; }                               \
  using array_type = float[2];                                                                \
  array_type& data() { return *reinterpret_cast<array_type*>(&x); }

#define IM_VEC4_CLASS_EXTRA                                                                       \
  constexpr ImVec4(glm::vec2 value, float _z, float _w) : x(value.x), y(value.y), z(_z), w(_w) {} \
  constexpr ImVec4(glm::vec3 value, float _w) : x(value.x), y(value.y), z(value.z), w(_w) {}      \
  constexpr ImVec4(glm::vec4 value) : x(value.x), y(value.y), z(value.z), w(value.w) {}           \
  explicit(false) operator glm::vec4() const { return {x, y, z, w}; }                             \
  explicit(false) operator glm::vec3() const { return {x, y, z}; }                                \
  using array_type = float[4];                                                                    \
  array_type& data() { return *reinterpret_cast<array_type*>(&x); }

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>
#include <bgfx/bgfx.h>
#include <gsl/gsl>
#include <big2/imgui/imgui_frame_scoped.h>
#include <big2/imgui/imgui_context_wrapper.h>

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
void ImGuiInit(gsl::not_null<ImGuiContext *> context, gsl::not_null<GLFWwindow *> window, bgfx::ViewId view_id, bool use_default_callbacks = false);

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

}
#endif // BIG2_IMGUI_ENABLED

#endif //BIG2_STACK_IMGUI_IMGUI_H_
