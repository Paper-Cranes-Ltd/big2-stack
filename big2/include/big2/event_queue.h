//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_BIG2_GLFW_EVENT_QUEUE_H_
#define BIG2_STACK_BIG2_GLFW_EVENT_QUEUE_H_

#include <gsl/gsl>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <optional>
#include <GLFW/glfw3.h>
#include <variant>

namespace big2 {

enum class ButtonPressState : std::int32_t {
  Pressed = GLFW_PRESS,
  Released = GLFW_RELEASE,
  Repeated = GLFW_REPEAT
};

struct GlfwGlobalEvent {
  struct MonitorConnectChange { GLFWmonitor *monitor; bool connected; };
  struct GamepadConnectChange { std::int32_t id; bool connected; };

  using EventData = std::variant<MonitorConnectChange, GamepadConnectChange>;
  EventData data;
};

struct GlfwEvent {
  struct WindowMoved { glm::ivec2 position; };
  struct WindowResized { glm::ivec2 new_size; };
  struct WindowClosed {};
  struct WindowRefresh {};
  struct WindowFocusChange { bool focused; };
  struct WindowIconifyChange { bool iconified; };
  struct WindowMaximizeChange { bool maximized; };
  struct WindowContentScaleChange { glm::vec2 scale; };
  struct FrameBufferResized { glm::ivec2 new_size; };
  struct MouseButton { std::int32_t button; std::int32_t mods; ButtonPressState state; };
  struct MousePosition { glm::vec2 position; };
  struct MouseEnterChange { bool entered; };
  struct Scroll { glm::vec2 scroll; };
  struct KeyboardButton { std::int32_t key; std::int32_t scan_code; std::int32_t mods; ButtonPressState state; };
  struct CharEntered { std::uint32_t character; };
  struct FileDrop { std::vector<std::string> files; };

  using EventData = std::variant<
      WindowMoved,
      WindowResized,
      WindowClosed,
      WindowRefresh,
      WindowFocusChange,
      WindowIconifyChange,
      WindowMaximizeChange,
      WindowContentScaleChange,
      FrameBufferResized,
      MouseButton,
      MousePosition,
      MouseEnterChange,
      Scroll,
      KeyboardButton,
      CharEntered,
      FileDrop
  >;

  explicit GlfwEvent(gsl::not_null<GLFWwindow*> window);

  template<typename T>
  [[nodiscard]] bool Is() const {
    return std::holds_alternative<T>(data);
  }

  template<typename T>
  [[nodiscard]] const T &Get() const {
    return std::get<T>(data);
  }

  template<typename T>
  [[nodiscard]] T &Get() {
    return const_cast<T &>(const_cast<const GlfwEvent *>(this)->Get<T>());
  }

  EventData data;
  gsl::not_null<GLFWwindow *> window;
};

namespace GlfwEventQueue {
/**
 * @brief Initializes the event queue and attaches global event handlers
 */
void Initialize();

/**
 * @brief Connects window handlers to the event queue.
 * @param window An initialized window handle
 */
void ConnectWindow(gsl::not_null<GLFWwindow *> window);

/**
 * @brief Gives back a reference to the global events that happened in the current frame.
 * These are events like monitor or gamepad being connected or disconnected.
 * Use GlfwEventQueue::PollEvents() to update the list each frame.
 */
gsl::span<GlfwGlobalEvent> GrabGlobalEvents();

/**
 * @brief Gives back a reference to the events that happened for a given window in the current frame.
 * Use GlfwEventQueue::PollEvents() to update the list each frame.
 * @param window An initialized window handle
 */
gsl::span<GlfwEvent> GrabEvents(gsl::not_null<GLFWwindow *> window);

/**
 * @brief Polls the events and stores them in static memory until called again.
 * @details Upon calling it the previous events will be cleared.
 * Call each frame to update the event queue.
 * Make sure that it is always being called by one thread since this allows for parallel optimizations.
 */
void PollEvents();

#if BIG2_IMGUI_ENABLED
/**
 * @brief Updates ImGui with the events for a given window only.
 * Make sure the ImGui has the correct ImGuiContext set before calling this function.
 * @param window An initialized window handle
 * @param window_events The events connected with the current window
 */
void UpdateImGuiEvents(gsl::not_null<GLFWwindow *> window, gsl::span<GlfwEvent> window_events);

/**
 * @brief Same as UpdateImGuiEvents(gsl::not_null<GLFWwindow *> window, gsl::span<GlfwEvent> window_events).
 * Will call GrabEvents() for the window that is passed.
 * @param window An initialized window handle
 */
inline void UpdateImGuiEvents(gsl::not_null<GLFWwindow *> window) { UpdateImGuiEvents(window, GrabEvents(window)); }
#endif
}

}

#endif //BIG2_STACK_BIG2_GLFW_EVENT_QUEUE_H_
