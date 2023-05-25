cmake_minimum_required(VERSION 3.22)
project(big2-stack VERSION 0.0.4)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

set(USE_FOLDERS TRUE)
set(CMAKE_FOLDER "BIG2")

# Requirements

find_package(Git)
if (NOT GIT_FOUND)
    message(FATAL_ERROR "Git needs to be installed to pull the dependencies")
else()
    string(REGEX MATCH "^[0-9]+[.][0-9]+" GIT_VERSION "${GIT_VERSION_STRING}")
    if("${GIT_VERSION}" LESS "2.35")
        message(FATAL_ERROR "Git version of 2.35 is needed for patch application")
    endif()
endif()

# Options

option(BIG2_BUILD_EXAMPLES "Whether or not to build examples with this stack" "${PROJECT_IS_TOP_LEVEL}")
option(BIG2_BUILD_DOCS "Whether or not to generate documentation" "${PROJECT_IS_TOP_LEVEL}")
option(BIG2_INCLUDE_IMGUI "Whether or not to add imgui into the mix" ON)

# Dependencies
include(FetchContent)

set(BIG2_COMMON_DEPS_OPTIONS)
list(APPEND BIG2_COMMON_DEPS_OPTIONS GIT_SHALLOW TRUE)
list(APPEND BIG2_COMMON_DEPS_OPTIONS GIT_PROGRESS TRUE)
list(APPEND BIG2_COMMON_DEPS_OPTIONS USES_TERMINAL_DOWNLOAD TRUE)
list(APPEND BIG2_COMMON_DEPS_OPTIONS GIT_REMOTE_UPDATE_STRATEGY CHECKOUT)
list(APPEND BIG2_COMMON_DEPS_OPTIONS GIT_SUBMODULES_RECURSE TRUE)

include(cmake/glfw.cmake)
include(cmake/bgfx.cmake)
include(cmake/glm.cmake)
include(cmake/spdlog.cmake)
include(cmake/microsoft-gsl.cmake)
include(cmake/utils.cmake)

if("${BIG2_INCLUDE_IMGUI}")
    include(cmake/imgui.cmake)
endif()

add_subdirectory(big2)

if("${BIG2_BUILD_EXAMPLES}")
    add_subdirectory(examples)
endif()

include(cmake/docs.cmake)
