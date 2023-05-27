//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//
#include <big2/event_queue.h>
#include <big2/macros.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <algorithm>
#include <backends/imgui_impl_glfw.h>
#include <execution>
#include <functional>
#include <concepts>
#include <ranges>

namespace big2 {

namespace GlfwEventQueue {

static std::vector<GlfwGlobalEvent> global_events;
static std::vector<GlfwEvent> events;

static void OnWindowMoved(GLFWwindow *window, std::int32_t x, std::int32_t y) {
  GlfwEvent event(window);
  event.data = GlfwEvent::WindowMoved{.position = glm::ivec2(x, y),};
  events.push_back(event);
}

static void OnWindowResized(GLFWwindow *window, std::int32_t width, std::int32_t height) {
  GlfwEvent event(window);
  event.data = GlfwEvent::WindowResized{.new_size = glm::ivec2(width, height),};
  events.push_back(event);
}

static void OnWindowClosed(GLFWwindow *window) {
  GlfwEvent event(window);
  event.data = GlfwEvent::WindowClosed{};
  events.push_back(event);
}

static void OnWindowRefresh(GLFWwindow *window) {
  GlfwEvent event(window);
  event.data = GlfwEvent::WindowRefresh{};
  events.push_back(event);
}

static void OnWindowFocusChange(GLFWwindow *window, std::int32_t focused) {
  GlfwEvent event(window);
  event.data = GlfwEvent::WindowFocusChange{.focused = static_cast<bool>(focused),};
  events.push_back(event);
}

static void OnWindowIconifyChange(GLFWwindow *window, std::int32_t iconified) {
  GlfwEvent event(window);
  event.data = GlfwEvent::WindowIconifyChange{.iconified = static_cast<bool>(iconified),};
  events.push_back(event);
}

static void OnFrameBufferResized(GLFWwindow *window, std::int32_t width, std::int32_t height) {
  GlfwEvent event(window);
  event.data = GlfwEvent::FrameBufferResized{.new_size = glm::ivec2(width, height),};
  events.push_back(event);
}

static void OnMouseButtonEvent(GLFWwindow *window, std::int32_t button, std::int32_t action, std::int32_t mods) {
  GlfwEvent event(window);
  event.data = GlfwEvent::MouseButton{.button = button, .mods = mods, .state =  static_cast<ButtonPressState>(action),};
  events.push_back(event);
}

static void OnMousePositionEvent(GLFWwindow *window, std::double_t x, std::double_t y) {
  GlfwEvent event(window);
  event.data = GlfwEvent::MousePosition{.position = glm::vec2(x, y),};
  events.push_back(event);
}

static void OnMouseEnterChange(GLFWwindow *window, std::int32_t entered) {
  GlfwEvent event(window);
  event.data = GlfwEvent::MouseEnterChange{.entered = static_cast<bool>(entered),};
  events.push_back(event);
}

static void OnScroll(GLFWwindow *window, std::double_t x, std::double_t y) {
  GlfwEvent event(window);
  event.data = GlfwEvent::Scroll{.scroll = glm::vec2(x, y),};
  events.push_back(event);
}

static void OnKeyboardButton(GLFWwindow *window, std::int32_t key, std::int32_t scan_code, std::int32_t action, std::int32_t mods) {
  GlfwEvent event(window);
  event.data = GlfwEvent::KeyboardButton{.key = key, .scan_code = scan_code, .mods = mods, .state = static_cast<ButtonPressState>(action),};
  events.push_back(event);
}

static void OnCharEntered(GLFWwindow *window, std::uint32_t codepoint) {
  GlfwEvent event(window);
  event.data = GlfwEvent::CharEntered{.character = codepoint};
  events.push_back(event);
}

static void OnMonitorConnectChange(GLFWmonitor *monitor, std::int32_t action) {
  GlfwGlobalEvent event;
  event.data = GlfwGlobalEvent::MonitorConnectChange{.monitor = monitor, .connected = static_cast<bool>(action),};
  global_events.push_back(event);
}

static void OnFileDrop(GLFWwindow *window, std::int32_t count, gsl::czstring raw_paths[]) {
  gsl::span<gsl::czstring> paths_span(raw_paths, count);

  GlfwEvent event(window);
  event.data = GlfwEvent::FileDrop{};

  auto &drop_data = std::get<GlfwEvent::FileDrop>(event.data);
  std::transform(paths_span.begin(), paths_span.end(), std::back_inserter(drop_data.files),
                 [](gsl::czstring x) { return std::string(x); });
  events.push_back(event);
}

static void OnWindowMaximizeChange(GLFWwindow *window, std::int32_t maximized) {
  GlfwEvent event(window);
  event.data = GlfwEvent::WindowMaximizeChange{.maximized = static_cast<bool>(maximized),};
  events.push_back(event);
}

static void OnWindowContentScaleChange(GLFWwindow *window, std::float_t xscale, std::float_t yscale) {
  GlfwEvent event(window);
  event.data = GlfwEvent::WindowContentScaleChange{.scale = glm::vec2(xscale, yscale),};
  events.push_back(event);
}

static void OnGamepadConnectChange(std::int32_t id, std::int32_t action) {
  GlfwGlobalEvent event;
  event.data = GlfwGlobalEvent::GamepadConnectChange{.id = id, .connected = static_cast<bool>(action),};
  global_events.push_back(event);
}

void Initialize() {
  glfwSetMonitorCallback(OnMonitorConnectChange);
  glfwSetJoystickCallback(OnGamepadConnectChange);
}

void ConnectWindow(gsl::not_null<GLFWwindow *> window) {
  glfwSetWindowPosCallback(window, OnWindowMoved);
  glfwSetWindowSizeCallback(window, OnWindowResized);
  glfwSetWindowCloseCallback(window, OnWindowClosed);
  glfwSetWindowRefreshCallback(window, OnWindowRefresh);
  glfwSetWindowFocusCallback(window, OnWindowFocusChange);
  glfwSetWindowIconifyCallback(window, OnWindowIconifyChange);
  glfwSetFramebufferSizeCallback(window, OnFrameBufferResized);
  glfwSetMouseButtonCallback(window, OnMouseButtonEvent);
  glfwSetCursorPosCallback(window, OnMousePositionEvent);
  glfwSetCursorEnterCallback(window, OnMouseEnterChange);
  glfwSetScrollCallback(window, OnScroll);
  glfwSetKeyCallback(window, OnKeyboardButton);
  glfwSetCharCallback(window, OnCharEntered);
  glfwSetDropCallback(window, OnFileDrop);
  glfwSetWindowMaximizeCallback(window, OnWindowMaximizeChange);
  glfwSetWindowContentScaleCallback(window, OnWindowContentScaleChange);
}

gsl::span<GlfwGlobalEvent> GrabGlobalEvents() {
  const gsl::span<GlfwGlobalEvent> result(global_events);
  return result;
}

gsl::span<GlfwEvent> GrabEvents(gsl::not_null<GLFWwindow *> window) {
  auto first_event_it = std::find_if(events.begin(), events.end(),
                                     [window](const GlfwEvent &e) { return e.window == window.get(); });

  if (first_event_it == events.end()) {
    return {};
  }

  auto last_event_it = std::find_if(first_event_it, events.end(),
                                    [window](const GlfwEvent &e) { return e.window != window.get(); });

  return {&*first_event_it, static_cast<size_t>(last_event_it - first_event_it)};
}

static void SortEventsByWindow() {
  using TRef = typename decltype(events)::reference;
  const std::invocable<TRef, TRef> auto &&sorter = [](TRef left, TRef right) noexcept { return left.window > right.window; };
  std::sort(std::execution::par_unseq, events.begin(), events.end(), sorter);
}

void PollEvents() {
  events.clear();
  glfwPollEvents();
  SortEventsByWindow();
}

void UpdateImGuiEvents(gsl::not_null<GLFWwindow *> window, gsl::span<GlfwEvent> window_events) {
  using TRef = typename decltype(events)::reference;
  const std::invocable<TRef> auto &&is_relevant = [](TRef event) {
    return event.Is<GlfwEvent::WindowFocusChange>()
        || event.Is<GlfwEvent::MouseEnterChange>()
        || event.Is<GlfwEvent::MousePosition>()
        || event.Is<GlfwEvent::MouseButton>()
        || event.Is<GlfwEvent::Scroll>()
        || event.Is<GlfwEvent::KeyboardButton>()
        || event.Is<GlfwEvent::CharEntered>();
  };

  auto relevant_event_processor = [window](TRef event) {
    std::visit([window](auto &data) {
      using T = std::decay_t<decltype(data)>;

      if constexpr (std::is_same_v<T, GlfwEvent::WindowFocusChange>) {
        ImGui_ImplGlfw_WindowFocusCallback(window, data.focused);
      }
      if constexpr (std::is_same_v<T, GlfwEvent::MouseEnterChange>) {
        ImGui_ImplGlfw_CursorEnterCallback(window, data.entered);
      }
      if constexpr (std::is_same_v<T, GlfwEvent::MousePosition>) {
        const glm::vec2 position = data.position;
        ImGui_ImplGlfw_CursorPosCallback(window, position.x, position.y);
      }
      if constexpr (std::is_same_v<T, GlfwEvent::MouseButton>) {
        ImGui_ImplGlfw_MouseButtonCallback(window, data.button, static_cast<std::int32_t>(data.state), data.mods);
      }
      if constexpr (std::is_same_v<T, GlfwEvent::Scroll>) {
        const glm::vec2 scroll = data.scroll;
        ImGui_ImplGlfw_ScrollCallback(window, scroll.x, scroll.y);
      }
      if constexpr (std::is_same_v<T, GlfwEvent::KeyboardButton>) {
        ImGui_ImplGlfw_KeyCallback(window, data.key, data.scan_code, static_cast<std::int32_t>(data.state), data.mods);
      }
      if constexpr (std::is_same_v<T, GlfwEvent::CharEntered>) {
        ImGui_ImplGlfw_CharCallback(window, data.character);
      }

    }, event.data);
  };

  std::ranges::for_each(window_events | std::views::filter(is_relevant), relevant_event_processor);
}

}

GlfwEvent::GlfwEvent(gsl::not_null<GLFWwindow *> window) : window(window) {

}

}
