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

namespace big2
{
[[nodiscard]] gsl::span<GLFWmonitor*> GetMonitors();
[[nodiscard]] glm::ivec2 GetMonitorResolution(gsl::not_null<GLFWmonitor *> monitor);
[[nodiscard]] glm::ivec2 GetMonitorPosition(gsl::not_null<GLFWmonitor *> monitor);
[[nodiscard]] std::int32_t GetMonitorRefreshRate(gsl::not_null<GLFWmonitor *> monitor);

[[nodiscard]] glm::ivec2 GetWindowSize(gsl::not_null<GLFWwindow *> window);
}

#endif //BIG2_STACK_BIG2_GLFW_UTILS_H_
