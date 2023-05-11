//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_PRIVATE_NATIVE_WINDOW_H_
#define BIG2_STACK_PRIVATE_NATIVE_WINDOW_H_

#include <bx/bx.h>
#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3native.h>

#endif //BIG2_STACK_PRIVATE_NATIVE_WINDOW_H_
