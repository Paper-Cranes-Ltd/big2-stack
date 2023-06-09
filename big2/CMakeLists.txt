project(BIG2)

set(CMAKE_CXX_STANDARD 17)

add_shaders_directory(shaders SHADERS_TARGET_NAME)

set(BIG2_SOURCES)

# include/
list(APPEND BIG2_SOURCES include/big2/bgfx/embedded_shader.h)
list(APPEND BIG2_SOURCES include/big2/bgfx/bgfx_frame_buffer_scoped.h)
list(APPEND BIG2_SOURCES include/big2/bgfx/bgfx_view_scoped.h)
list(APPEND BIG2_SOURCES include/big2/bgfx/bgfx_utils.h)
list(APPEND BIG2_SOURCES include/big2/event_queue.h)
list(APPEND BIG2_SOURCES include/big2/glfw/glfw_utils.h)
list(APPEND BIG2_SOURCES include/big2/glfw/glfw_initialization_scoped.h)
list(APPEND BIG2_SOURCES include/big2/macros.h)
list(APPEND BIG2_SOURCES include/big2/void_ptr.h)
list(APPEND BIG2_SOURCES include/big2/asserts.h)
list(APPEND BIG2_SOURCES include/big2.h)

# src/
list(APPEND BIG2_SOURCES src/glfw/glfw_utils.cpp)
list(APPEND BIG2_SOURCES src/glfw/glfw_initialization_scoped.cpp)
list(APPEND BIG2_SOURCES src/glfw/glfw_window_scoped.cpp)
list(APPEND BIG2_SOURCES src/bgfx/bgfx_utils.cpp)
list(APPEND BIG2_SOURCES src/bgfx/bgfx_frame_buffer_scoped.cpp)
list(APPEND BIG2_SOURCES src/bgfx/bgfx_view_scoped.cpp)
list(APPEND BIG2_SOURCES src/event_queue.cpp)
list(APPEND BIG2_SOURCES src/native_window.h)
list(APPEND BIG2_SOURCES src/id_manager.h)
list(APPEND BIG2_SOURCES src/asserts.cpp)
list(APPEND BIG2_SOURCES src/big2.cpp)

if("${BIG2_INCLUDE_IMGUI}")
    list(APPEND BIG2_SOURCES include/big2/imgui/imgui.h)
    list(APPEND BIG2_SOURCES include/big2/imgui/imgui_impl_bgfx.h)
    list(APPEND BIG2_SOURCES include/big2/imgui/imgui_frame_scoped.h)
    list(APPEND BIG2_SOURCES include/big2/imgui/imgui_single_context_scoped.h)

    list(APPEND BIG2_SOURCES src/imgui/imgui.cpp)
    list(APPEND BIG2_SOURCES src/imgui/imgui_frame_scoped.cpp)
    list(APPEND BIG2_SOURCES src/imgui/imgui_single_context_scoped.cpp)
    list(APPEND BIG2_SOURCES src/imgui/imgui_impl_bgfx.cpp)
endif()

add_library(${PROJECT_NAME} ${BIG2_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(${PROJECT_NAME} PUBLIC glfw bx bimg bgfx)
target_link_libraries(${PROJECT_NAME} PUBLIC Microsoft.GSL::GSL)
target_link_libraries(${PROJECT_NAME} PUBLIC glm)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)
target_link_libraries(${PROJECT_NAME} PUBLIC "${SHADERS_TARGET_NAME}")

if("${BIG2_INCLUDE_IMGUI}")
    target_link_libraries(${PROJECT_NAME} PUBLIC imgui)
    target_compile_definitions(${PROJECT_NAME} PUBLIC BIG2_IMGUI_ENABLED=1)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC BIG2_IMGUI_ENABLED=0)
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC GLFW_INCLUDE_NONE)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
