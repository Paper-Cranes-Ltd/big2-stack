//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_GLFW_UTILS_H_
#define BIG2_STACK_GLFW_UTILS_H_

#include <glm/glm.hpp>
#include <gsl/gsl>
#include <big2/glfw/glfw_initialization_scoped.h>
#include <big2/glfw/glfw_window_scoped.h>

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
 * @brief Returns the window resolution (framebuffer size)
 * @return  A 2D vector where x is the width and y is the height of the window
 */
[[nodiscard]] glm::ivec2 GetWindowResolution(gsl::not_null<GLFWwindow *> window);

/**
 * @brief Gets the platform native window handle.
 * @param window An initialized window handle
 */
[[nodiscard]] void *GetNativeWindowHandle(gsl::not_null<GLFWwindow *> window);

/**
 * @brief An error callback that will log the error.
 */
void GlfwErrorCallback(std::int32_t error, gsl::czstring description);

}

#endif //BIG2_STACK_GLFW_UTILS_H_
