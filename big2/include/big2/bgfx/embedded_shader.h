//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_EMBEDDED_SHADER_H_
#define BIG2_STACK_EMBEDDED_SHADER_H_

#include <bx/bx.h>
#include <bgfx/embedded_shader.h>

#if BX_PLATFORM_LINUX
#undef BGFX_EMBEDDED_SHADER_METAL
#undef BGFX_EMBEDDED_SHADER_DXBC
#undef BGFX_EMBEDDED_SHADER_DX9BC
#undef BGFX_EMBEDDED_SHADER_NVN

#define BGFX_EMBEDDED_SHADER_METAL(...)
#define BGFX_EMBEDDED_SHADER_DXBC(...)
#define BGFX_EMBEDDED_SHADER_DX9BC(...)
#define BGFX_EMBEDDED_SHADER_NVN(...)
#endif

#endif //BIG2_STACK_EMBEDDED_SHADER_H_
