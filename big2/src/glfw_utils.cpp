//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#include <big2/glfw_utils.h>
#include <GLFW/glfw3.h>

namespace big2
{

gsl::span<GLFWmonitor *> GetMonitors()
{
    std::int32_t count;
    GLFWmonitor **monitors = glfwGetMonitors(&count);
    return {monitors, static_cast<std::size_t>(count)};
}

glm::ivec2 GetMonitorResolution(gsl::not_null<GLFWmonitor *> monitor)
{
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    return {mode->width, mode->height};
}

glm::ivec2 GetMonitorPosition(gsl::not_null<GLFWmonitor *> monitor)
{
    glm::ivec2 position;
    glfwGetMonitorPos(monitor, &position.x, &position.y);
    return position;
}

std::int32_t GetMonitorRefreshRate(gsl::not_null<GLFWmonitor *> monitor)
{
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    return mode->refreshRate;
}

glm::ivec2 GetWindowSize(gsl::not_null<GLFWwindow *> window)
{
    glm::ivec2 window_size;
    glfwGetWindowSize(window, &window_size.x, &window_size.y);
    return window_size;
}

}