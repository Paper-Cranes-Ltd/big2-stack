//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#include <big2/glfw_utils.h>
#include <GLFW/glfw3.h>
#include <bx/bx.h>

#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3native.h>

namespace big2
{


void SetNativeWindowData(bgfx::Init &init_obj, GLFWwindow *window)
{
#if BX_PLATFORM_LINUX
    init_obj.platformData.ndt = glfwGetX11Display();
    init_obj.platformData.nwh = reinterpret_cast<void *>(glfwGetX11Window(window));
#elif BX_PLATFORM_OSX
    init_obj.platformData.nwh = glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
    init_obj.platformData.nwh = glfwGetWin32Window(window);
#endif
}

gsl::span<GLFWmonitor*> GetMonitors()
{
    std::int32_t count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);
    return {monitors, static_cast<std::size_t>(count) };
}

}
