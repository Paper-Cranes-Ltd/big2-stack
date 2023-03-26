fetchcontent_declare(
        imgui
        GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
        GIT_TAG "v1.89.4"
        ${BIG2_COMMON_DEPS_OPTIONS}
)

fetchcontent_populate(imgui)

set(IMGUI_SOURCES)
list(APPEND IMGUI_SOURCES "${imgui_SOURCE_DIR}/imgui.cpp")
list(APPEND IMGUI_SOURCES "${imgui_SOURCE_DIR}/imgui_demo.cpp")
list(APPEND IMGUI_SOURCES "${imgui_SOURCE_DIR}/imgui_draw.cpp")
list(APPEND IMGUI_SOURCES "${imgui_SOURCE_DIR}/imgui_tables.cpp")
list(APPEND IMGUI_SOURCES "${imgui_SOURCE_DIR}/imgui_widgets.cpp")
list(APPEND IMGUI_SOURCES "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h")
list(APPEND IMGUI_SOURCES "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp")

add_library(imgui ${IMGUI_SOURCES})
target_link_libraries(imgui PUBLIC glfw bx bimg bgfx)
target_include_directories(imgui PUBLIC "${imgui_SOURCE_DIR}")
set_target_properties( imgui PROPERTIES FOLDER "ImGui")