cmake_minimum_required(VERSION 3.22)
project(big2-stack VERSION 0.0.9)

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
include(CMakeDependentOption)

option(BIG2_BUILD_EXAMPLES "Whether or not to build examples with this stack" "${PROJECT_IS_TOP_LEVEL}")
option(BIG2_BUILD_DOCS "Whether or not to generate documentation" "${PROJECT_IS_TOP_LEVEL}")
option(BIG2_INCLUDE_IMGUI "Whether or not to add imgui into the mix" ON)
cmake_dependent_option(BIG2_USE_IMGUI_DOCKING "Will use the docking branch of ImGui" OFF "BIG2_INCLUDE_IMGUI" OFF)

if(BIG2_USE_IMGUI_DOCKING)
    set(IMGUI_VERSION "docking")
else()
    set(IMGUI_VERSION "v1.89.4")
endif ()

include(cmake/utils.cmake)

# Dependencies
include(external/cmake-snippets/JsonDependency.cmake)
json_config_dependencies(FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake-config.json" MEMBER_SELECTOR "dependencies")

if("${BIG2_INCLUDE_IMGUI}")
    include(cmake/imgui.cmake)
endif()

add_subdirectory(big2)

if(BIG2_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(BIG2_BUILD_DOCS)
    include(cmake/docs.cmake)
endif ()
