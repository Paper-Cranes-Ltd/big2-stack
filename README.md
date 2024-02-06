![Logo](./press/logo-color.png)

# The BIG<sup>2</sup> Stack

[![License - The Unlicense](https://badgen.net/badge/License/The%20Unlicense/blue)](https://github.com/Paper-Cranes-Ltd/big2-stack/blob/main/LICENSE)
[![Documentation](https://badgen.net/badge/icon/Documentation/pink?icon=wiki&label)](https://paper-cranes-ltd.github.io/big2-stack/)
[![ThatOneGameDev.com](https://badgen.net/badge/@/ThatOneGameDev.com/6633CC/?icon=awesome&label)](https://thatonegamedev.com)
[![Buy me a coffee](https://badgen.net/badge/icon/Buy%20me%20a%20coffee/FF6600/?icon=buymeacoffee&label)](https://www.buymeacoffee.com/object71)

[![Linux](https://github.com/Paper-Cranes-Ltd/big2-stack/actions/workflows/linux.yml/badge.svg)](https://github.com/Paper-Cranes-Ltd/big2-stack/actions/workflows/linux.yml)
[![MacOS](https://github.com/Paper-Cranes-Ltd/big2-stack/actions/workflows/macos.yml/badge.svg)](https://github.com/Paper-Cranes-Ltd/big2-stack/actions/workflows/macos.yml)
[![Windows](https://github.com/Paper-Cranes-Ltd/big2-stack/actions/workflows/windows.yml/badge.svg)](https://github.com/Paper-Cranes-Ltd/big2-stack/actions/workflows/windows.yml)

This is a middleware repository that eases up the work with [BGFX](https://github.com/bkaradzic/bgfx), [GLFW](https://github.com/glfw/glfw), [IMGUI](https://github.com/ocornut/imgui) and [GLM](https://github.com/g-truc/glm).
It is more or less inspired by other repositories like [bigg](https://github.com/JoshuaBrookover/bigg) or [bigger](https://github.com/yuki-koyama/bigger). The general idea is to have a setup that is easy to work with and is easy to have cross-platform. This project is based on CMake since this is my favourite and is also the most popular project management tool and you can easily include it in your own project as a subdirectory or using fetch content.

> <div style="color: orange;">⚠ Warning</div>
> This library is still largely under development and the interface might change drastically until version "v0.2.x" where I will start to stabilize the functions and not make immediate breaking changes.

## Usage

You can check the intro article on [my blog](https://thatonegamedev.com/cpp/the-big2-stack/) but in general to use this in your cmake project you could clone it to a subdirectory or use fetch content:

```cmake
include(FetchContent)

fetchcontent_declare(
        big2
        GIT_REPOSITORY "https://github.com/Paper-Cranes-Ltd/big2-stack.git"
        GIT_TAG "v0.0.9"
        GIT_SHALLOW TRUE
)

fetchcontent_makeavailable(big2)

# ...

add_executable(Main main.cpp)
target_link_libraries(Main PUBLIC big2)
```

## Dependencies

This project depends on a few other projects:

- [Microsoft.GSL](https://github.com/microsoft/GSL)
- [bgfx.cmake](https://github.com/bkaradzic/bgfx)
- [glfw3](https://github.com/glfw/glfw)
- [glm](https://github.com/g-truc/glm)
- [spdlog](https://github.com/gabime/spdlog)
- [Dear ImGui](https://github.com/ocornut/imgui) *(optional)*
