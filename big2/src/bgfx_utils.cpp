//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//
#include <big2/bgfx_utils.h>
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

void * GetNativeWindowHandle(gsl::not_null<GLFWwindow *>window)
{
#if BX_PLATFORM_LINUX
    return reinterpret_cast<void *>(glfwGetX11Window(window));
#elif BX_PLATFORM_OSX
    return glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
    return glfwGetWin32Window(window);
#else
    return nullptr;
#endif
}

void SetNativeData(bgfx::Init &init_obj)
{
#if BX_PLATFORM_LINUX
    init_obj.platformData.ndt = glfwGetX11Display();
#endif
}

void SetNativeWindowData(bgfx::Init &init_obj, gsl::not_null<GLFWwindow *> window)
{
    SetNativeData(init_obj);
    init_obj.platformData.nwh = GetNativeWindowHandle(window);
}

bgfx::FrameBufferHandle CreateWindowFramebuffer(gsl::not_null<GLFWwindow *> window)
{
    glm::ivec2 window_size = GetWindowSize(window);
    return bgfx::createFrameBuffer(GetNativeWindowHandle(window), window_size.x, window_size.y);
}

}
