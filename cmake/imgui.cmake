fetchcontent_getproperties(
        imgui
        SOURCE_DIR IMGUI_SOURCE_DIR
)

set(IMGUI_SOURCES)
list(APPEND IMGUI_SOURCES "${IMGUI_SOURCE_DIR}/imgui.h")
list(APPEND IMGUI_SOURCES "${IMGUI_SOURCE_DIR}/imconfig.h")
list(APPEND IMGUI_SOURCES "${IMGUI_SOURCE_DIR}/imgui_internal.h")
list(APPEND IMGUI_SOURCES "${IMGUI_SOURCE_DIR}/imstb_rectpack.h")
list(APPEND IMGUI_SOURCES "${IMGUI_SOURCE_DIR}/imstb_textedit.h")
list(APPEND IMGUI_SOURCES "${IMGUI_SOURCE_DIR}/imstb_truetype.h")
list(APPEND IMGUI_SOURCES "${IMGUI_SOURCE_DIR}/imgui.cpp")
list(APPEND IMGUI_SOURCES "${IMGUI_SOURCE_DIR}/imgui_demo.cpp")
list(APPEND IMGUI_SOURCES "${IMGUI_SOURCE_DIR}/imgui_draw.cpp")
list(APPEND IMGUI_SOURCES "${IMGUI_SOURCE_DIR}/imgui_tables.cpp")
list(APPEND IMGUI_SOURCES "${IMGUI_SOURCE_DIR}/imgui_widgets.cpp")
list(APPEND IMGUI_SOURCES "${IMGUI_SOURCE_DIR}/backends/imgui_impl_glfw.h")
list(APPEND IMGUI_SOURCES "${IMGUI_SOURCE_DIR}/backends/imgui_impl_glfw.cpp")
list(APPEND IMGUI_SOURCES "${IMGUI_SOURCE_DIR}/misc/cpp/imgui_stdlib.h")
list(APPEND IMGUI_SOURCES "${IMGUI_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp")

add_library(imgui ${IMGUI_SOURCES})
target_link_libraries(imgui PUBLIC glfw bx bimg bgfx)
target_include_directories(imgui PUBLIC "${IMGUI_SOURCE_DIR}")
target_include_directories(imgui PUBLIC "${IMGUI_SOURCE_DIR}/misc/cpp")
set_target_properties(imgui PROPERTIES FOLDER "ImGui")
