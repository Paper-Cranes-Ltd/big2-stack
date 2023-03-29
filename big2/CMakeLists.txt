project(BIG2)

set(CMAKE_CXX_STANDARD 17)

add_shaders_directory(shaders SHADERS_TARGET_NAME)

set(BIG2_SOURCES)
list(APPEND BIG2_SOURCES include/big2.h)
list(APPEND BIG2_SOURCES include/big2/embedded_shader.h)
list(APPEND BIG2_SOURCES src/library.cpp)

if("${BIG2_INCLUDE_IMGUI}")
    list(APPEND BIG2_SOURCES include/big2/imgui_impl_bgfx.h)
    list(APPEND BIG2_SOURCES src/imgui_impl_bgfx.cpp)
endif()

add_library(${PROJECT_NAME} ${BIG2_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${PROJECT_NAME} PUBLIC glfw bx bimg bgfx)

if("${BIG2_INCLUDE_IMGUI}")
    target_link_libraries(${PROJECT_NAME} PUBLIC imgui)
    target_compile_definitions(${PROJECT_NAME} PUBLIC BIG2_IMGUI_ENABLED=1)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC BIG2_IMGUI_ENABLED=0)
endif()

if("${BIG2_INCLUDE_GLM}")
    target_link_libraries(${PROJECT_NAME} PUBLIC glm)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC "${SHADERS_TARGET_NAME}")
