//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_BIG2_GLFW_UTILS_H_
#define BIG2_STACK_BIG2_GLFW_UTILS_H_

#include <glm/glm.hpp>
#include <gsl/gsl>

struct GLFWwindow;
struct GLFWmonitor;

namespace big2 {

/**
 * @brief A wrapper over glfwGetMonitors() but returning a span over the raw pointers.
 * @return span over the monitor pointer array returned from glfwGetMonitors()
 * @ref glfwGetMonitors()
 */
[[nodiscard]] gsl::span<GLFWmonitor *> GetMonitors();

/**
 * @brief Returns the monitor resolution
 * @return A 2D vector where x is the width and y is the height of the monitor
 */
[[nodiscard]] glm::ivec2 GetMonitorResolution(gsl::not_null<GLFWmonitor *> monitor);

/**
 * @brief Returns the monitor position
 * @details This is a virtual position provided by the OS.
 * You can use it when setting window position to move the window inside the monitor.
 * @return A 2D vector indicating the virtual position of this monitor
 */
[[nodiscard]] glm::ivec2 GetMonitorPosition(gsl::not_null<GLFWmonitor *> monitor);

/**
 * @brief Returns the monitor refresh rate
 * @return An integer value indicating the monitor refresh rate
 */
[[nodiscard]] std::int32_t GetMonitorRefreshRate(gsl::not_null<GLFWmonitor *> monitor);

/**
 * @brief Returns the window size
 * @return  A 2D vector where x is the width and y is the height of the window
 */
[[nodiscard]] glm::ivec2 GetWindowSize(gsl::not_null<GLFWwindow *> window);

/**
 * @brief Gets the platform native window handle.
 * @param window An initialized window handle
 */
[[nodiscard]] void *GetNativeWindowHandle(gsl::not_null<GLFWwindow *> window);

/**
 * @brief An error callback that will log the error.
 */
void GlfwErrorCallback(std::int32_t error, gsl::czstring description);

/**
 * @brief A scoped initialization class for GLFW that will also terminate it.
 * @details This class will warn if terminated or initialized twice and will also set a default error callback that will log any errors.
 */
class GlfwInitializationScoped final {
 public:
  GlfwInitializationScoped();
  GlfwInitializationScoped(GlfwInitializationScoped &&) = default;
  GlfwInitializationScoped &operator=(GlfwInitializationScoped &&) = default;
  GlfwInitializationScoped(const GlfwInitializationScoped &) = delete;
  GlfwInitializationScoped &operator=(const GlfwInitializationScoped &) = delete;
  ~GlfwInitializationScoped();

 private:
  static inline bool is_initialized_ = false;
};

class GlfwWindowScoped final {
 public:
  GlfwWindowScoped(gsl::not_null<GLFWwindow *> window);
  GlfwWindowScoped(gsl::czstring title, glm::ivec2 size, GLFWmonitor *monitor = nullptr);
  GlfwWindowScoped(GlfwWindowScoped &&) = default;
  GlfwWindowScoped &operator=(GlfwWindowScoped &&) = default;
  GlfwWindowScoped(const GlfwWindowScoped &) = delete;
  GlfwWindowScoped &operator=(const GlfwWindowScoped &) = delete;
  ~GlfwWindowScoped();

  operator GLFWwindow *() { return window_; }
  operator gsl::not_null<GLFWwindow *>() { return window_; }

  [[nodiscard]] gsl::not_null<GLFWwindow *> GetWindow() const { return window_; }

 private:
  gsl::owner<GLFWwindow *> window_ = nullptr;
};

}

#endif //BIG2_STACK_BIG2_GLFW_UTILS_H_
