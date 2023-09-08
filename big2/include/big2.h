//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_BIG2_H_
#define BIG2_STACK_BIG2_H_

/// @brief In this namespace you will find all kinds of useful functions to use with the whole stack.
namespace big2 {}
/// @private
namespace big2::detail {}

#if BIG2_IMGUI_ENABLED
#include <big2/imgui/imgui.h>
#include <big2/imgui/imgui_app_extension.h>
#endif //BIG2_IMGUI_ENABLED

#include <bx/bx.h>
#include <bgfx/bgfx.h>

#include <big2/asserts.h>
#include <big2/app.h>
#include <big2/app_extension_base.h>
#include <big2/default_quit_condition_app_extension.h>
#include <big2/macros.h>
#include <big2/void_ptr.h>
#include <big2/glfw/glfw_utils.h>
#include <big2/event_queue.h>
#include <big2/bgfx/bgfx_utils.h>



#endif //BIG2_STACK_BIG2_H_
