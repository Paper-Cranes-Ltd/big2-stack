//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//
#include <big2/glfw/event_queue.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <algorithm>
#include <backends/imgui_impl_glfw.h>
#include <execution>

namespace big2 {

namespace GlfwEventQueue {

static std::vector<GlfwEvent> events;

static void OnWindowMoved(GLFWwindow *window, std::int32_t x, std::int32_t y) {
  GlfwEvent event;
  event.window = window;
  event.data = GlfwEvent::WindowMoved{glm::ivec2(x, y),};
  events.push_back(event);
}

static void OnWindowResized(GLFWwindow *window, std::int32_t width, std::int32_t height) {
  GlfwEvent event;
  event.window = window;
  event.data = GlfwEvent::WindowResized{glm::ivec2(width, height),};
  events.push_back(event);
}

static void OnWindowClosed(GLFWwindow *window) {
  GlfwEvent event;
  event.window = window;
  event.data = GlfwEvent::WindowClosed{};
  events.push_back(event);
}

static void OnWindowRefresh(GLFWwindow *window) {
  GlfwEvent event;
  event.window = window;
  event.data = GlfwEvent::WindowRefresh{};
  events.push_back(event);
}

static void OnWindowFocusChange(GLFWwindow *window, std::int32_t focused) {
  GlfwEvent event;
  event.window = window;
  event.data = GlfwEvent::WindowFocusChange{static_cast<bool>(focused),};
  events.push_back(event);
}

static void OnWindowIconifyChange(GLFWwindow *window, std::int32_t iconified) {
  GlfwEvent event;
  event.window = window;
  event.data = GlfwEvent::WindowIconifyChange{static_cast<bool>(iconified),};
  events.push_back(event);
}

static void OnFrameBufferResized(GLFWwindow *window, std::int32_t width, std::int32_t height) {
  GlfwEvent event;
  event.window = window;
  event.data = GlfwEvent::FrameBufferResized{glm::ivec2(width, height),};
  events.push_back(event);
}

static void OnMouseButtonEvent(GLFWwindow *window, std::int32_t button, std::int32_t action, std::int32_t mods) {
  GlfwEvent event;
  event.window = window;
  event.data = GlfwEvent::MouseButton{button, mods, static_cast<ButtonPressState>(action),};
  events.push_back(event);
}

static void OnMousePositionEvent(GLFWwindow *window, std::double_t x, std::double_t y) {
  GlfwEvent event;
  event.window = window;
  event.data = GlfwEvent::MousePosition{glm::vec2(x, y),};
  events.push_back(event);
}

static void OnMouseEnterChange(GLFWwindow *window, std::int32_t entered) {
  GlfwEvent event;
  event.window = window;
  event.data = GlfwEvent::MouseEnterChange{static_cast<bool>(entered),};
  events.push_back(event);
}

static void OnScroll(GLFWwindow *window, std::double_t x, std::double_t y) {
  GlfwEvent event;
  event.window = window;
  event.data = GlfwEvent::Scroll{glm::vec2(x, y),};
  events.push_back(event);
}

static void OnKeyboardButton(GLFWwindow *window, std::int32_t key, std::int32_t scan_code, std::int32_t action, std::int32_t mods) {
  GlfwEvent event;
  event.window = window;
  event.data = GlfwEvent::KeyboardButton{key, scan_code, mods, static_cast<ButtonPressState>(action),};
  events.push_back(event);
}

static void OnCharEntered(GLFWwindow *window, std::uint32_t codepoint) {
  GlfwEvent event;
  event.window = window;
  event.data = GlfwEvent::CharEntered{codepoint};
  events.push_back(event);
}

static void OnMonitorConnectChange(GLFWmonitor *monitor, std::int32_t action) {
  GlfwEvent event;
  event.data = GlfwEvent::MonitorConnectChange{monitor, static_cast<bool>(action),};
  events.push_back(event);
}

static void OnFileDrop(GLFWwindow *window, std::int32_t count, gsl::czstring raw_paths[]) {
  gsl::span<gsl::czstring> paths_span(raw_paths, count);

  GlfwEvent event;
  event.window = nullptr;
  event.data = GlfwEvent::FileDrop{};

  auto &drop_data = std::get<GlfwEvent::FileDrop>(event.data);
  std::transform(paths_span.begin(), paths_span.end(), std::back_inserter(drop_data.files),
                 [](gsl::czstring x) { return std::string(x); });
  events.push_back(event);
}

static void OnWindowMaximizeChange(GLFWwindow *window, std::int32_t maximized) {
  GlfwEvent event;
  event.window = window;
  event.data = GlfwEvent::WindowMaximizeChange{static_cast<bool>(maximized),};
  events.push_back(event);
}

static void OnWindowContentScaleChange(GLFWwindow *window, std::float_t xscale, std::float_t yscale) {
  GlfwEvent event;
  event.window = window;
  event.data = GlfwEvent::WindowContentScaleChange{glm::vec2(xscale, yscale),};
  events.push_back(event);
}

static void OnGamepadConnectChange(std::int32_t id, std::int32_t action) {
  GlfwEvent event;
  event.data = GlfwEvent::GamepadConnectChange{id, static_cast<bool>(action),};
  events.push_back(event);
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

gsl::span<GlfwEvent> GrabGlobalEvents() {
  auto first_event_it = std::find_if(std::execution::par_unseq, events.begin(), events.end(),
                                     [](const GlfwEvent &e) { return !e.window.has_value(); });

  if (first_event_it == events.end()) {
    return {};
  }

  auto last_event_it = std::find_if(std::execution::par_unseq, first_event_it, events.end(),
                                    [](const GlfwEvent &e) { return e.window.has_value(); });

  return {&*first_event_it, static_cast<size_t>(last_event_it - first_event_it)};
}

gsl::span<GlfwEvent> GrabEvents(gsl::not_null<GLFWwindow *> window) {
  auto first_event_it = std::find_if(std::execution::par_unseq, events.begin(), events.end(),
                                     [window](const GlfwEvent &e) { return e.window.has_value() && e.window.value() == window.get(); });

  if (first_event_it == events.end()) {
    return {};
  }

  auto last_event_it = std::find_if(std::execution::par_unseq, first_event_it, events.end(),
                                    [window](const GlfwEvent &e) { return e.window.has_value() && e.window.value() != window.get(); });

  return {&*first_event_it, static_cast<size_t>(last_event_it - first_event_it)};
}

static bool WindowPointerEventSorter(const GlfwEvent &left, const GlfwEvent &right) {
  if (!left.window.has_value()) {
    return false;
  }

  if (!right.window.has_value()) {
    return true;
  }

  return left.window.value() > right.window.value();
}

static void SortEventsByWindow() {
  std::sort(std::execution::par_unseq, events.begin(), events.end(), WindowPointerEventSorter);
}

void PollEvents() {
  events.clear();
  glfwPollEvents();
  SortEventsByWindow();
}

void UpdateImGuiEvents(gsl::not_null<GLFWwindow *> window, gsl::span<GlfwEvent> relevant_events) {
  for (const GlfwEvent &event : relevant_events) {
    switch (event.GetDataType()) {
      case GlfwEvent::EventDataEnum::WindowFocusChange: {
        ImGui_ImplGlfw_WindowFocusCallback(window, event.Get<GlfwEvent::WindowFocusChange>().focused);
        break;
      }
      case GlfwEvent::EventDataEnum::MouseEnterChange: {
        ImGui_ImplGlfw_CursorEnterCallback(window, event.Get<GlfwEvent::MouseEnterChange>().entered);
        break;
      }
      case GlfwEvent::EventDataEnum::MousePosition: {
        glm::vec2 position = event.Get<GlfwEvent::MousePosition>().position;
        ImGui_ImplGlfw_CursorPosCallback(window, position.x, position.y);
        break;
      }
      case GlfwEvent::EventDataEnum::MouseButton: {
        const auto &button_data = event.Get<GlfwEvent::MouseButton>();
        ImGui_ImplGlfw_MouseButtonCallback(window, button_data.button, static_cast<std::int32_t>(button_data.state), button_data.mods);
        break;
      }
      case GlfwEvent::EventDataEnum::Scroll: {
        glm::vec2 scroll = event.Get<GlfwEvent::Scroll>().scroll;
        ImGui_ImplGlfw_ScrollCallback(window, scroll.x, scroll.y);
        break;
      }
      case GlfwEvent::EventDataEnum::KeyboardButton: {
        const auto &button_data = event.Get<GlfwEvent::KeyboardButton>();
        ImGui_ImplGlfw_KeyCallback(window, button_data.key, button_data.scan_code, static_cast<std::int32_t>(button_data.state), button_data.mods);
        break;
      }
      case GlfwEvent::EventDataEnum::CharEntered: {
        ImGui_ImplGlfw_CharCallback(window, event.Get<GlfwEvent::CharEntered>().character);
        break;
      }
      default:break;
    }
  }
}

}
}
