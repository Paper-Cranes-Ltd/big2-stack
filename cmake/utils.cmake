function(add_shaders_directory SHADERS_DIR TARGET_OUT_VAR)
    get_filename_component(SHADERS_DIR "${SHADERS_DIR}" ABSOLUTE)
    get_filename_component(NAMESPACE "${CMAKE_CURRENT_SOURCE_DIR}" NAME_WE)

    if(NOT EXISTS "${SHADERS_DIR}")
        message(NOTICE "Shaders directory does not exist")
        return()
    endif()

    file(GLOB VERTEX_SHADER_FILES CONFIGURE_DEPENDS FOLLOW_SYMLINKS "${SHADERS_DIR}/*.sc")
    list(FILTER VERTEX_SHADER_FILES EXCLUDE REGEX "\.def\.sc$")
    list(FILTER VERTEX_SHADER_FILES INCLUDE REGEX "[\\\/]((vs_[^\\\/]*\.sc)|([^\\\/]*(\.vert)(\.sc)))$")

    file(GLOB FRAGMENT_SHADER_FILES CONFIGURE_DEPENDS FOLLOW_SYMLINKS "${SHADERS_DIR}/*.sc")
    list(FILTER FRAGMENT_SHADER_FILES EXCLUDE REGEX "\.def\.sc$")
    list(FILTER FRAGMENT_SHADER_FILES INCLUDE REGEX "[\\\/]((fs_[^\\\/]*\.sc)|([^\\\/]*(\.frag)(\.sc)))$")

    if(NOT VERTEX_SHADER_FILES AND NOT FRAGMENT_SHADER_FILES)
        message(NOTICE "No shader files in directory")
        return()
    endif()

    if(CMAKE_CROSSCOMPILING AND NOT("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "${CMAKE_SYSTEM_NAME}"))
        message(STATUS "Not compiling shaders during cross-compilation")
        return()
    endif()

    set(VARYING_DEF_LOCATION "${SHADERS_DIR}/varying.def.sc")
    if(NOT EXISTS "${VARYING_DEF_LOCATION}")
        message(WARNING "Varying def does not exist")
        return()
    endif()

    set(SHADERS_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/include/generated/shaders/${NAMESPACE}")

    file(MAKE_DIRECTORY "${SHADERS_OUT_DIR}")

    bgfx_compile_shaders(
            TYPE VERTEX
            SHADERS ${VERTEX_SHADER_FILES}
            VARYING_DEF "${VARYING_DEF_LOCATION}"
            OUTPUT_DIR "${SHADERS_OUT_DIR}"
            OUT_FILES_VAR VERTEX_OUTPUT_FILES
            INCLUDE_DIRS "${SHADERS_DIR}" "${BGFX_DIR}/src"
            AS_HEADERS
    )

    bgfx_compile_shaders(
            TYPE FRAGMENT
            SHADERS ${FRAGMENT_SHADER_FILES}
            VARYING_DEF "${VARYING_DEF_LOCATION}"
            OUTPUT_DIR "${SHADERS_OUT_DIR}"
            OUT_FILES_VAR FRAGMENT_OUTPUT_FILES
            INCLUDE_DIRS "${SHADERS_DIR}" "${BGFX_DIR}/src"
            AS_HEADERS
    )

    set(OUTPUT_FILES)
    list(APPEND OUTPUT_FILES ${VERTEX_OUTPUT_FILES})
    list(APPEND OUTPUT_FILES ${FRAGMENT_OUTPUT_FILES})

    list(LENGTH OUTPUT_FILES SHADER_COUNT)
    if(SHADER_COUNT EQUAL 0)
        return()
    endif()

    set(INCLUDE_ALL_HEADER "")
    foreach(OUTPUT_FILE IN LISTS OUTPUT_FILES)
        get_filename_component(OUTPUT_FILENAME "${OUTPUT_FILE}" NAME)
        # string(APPEND INCLUDE_ALL_HEADER "#include <generated/shaders/${NAMESPACE}/${OUTPUT_FILENAME}>\n")
        
        # -- NOTE should be like
        # // main.cpp
        # #ifndef SHADERS_H_big2_fs_ocornut_imgui
        # #define SHADERS_H_big2_fs_ocornut_imgui
        # #include <glsl/vs.sc.bin.h>
        # #include <essl/vs.sc.bin.h>
        # #include <spirv/vs.sc.bin.h>
        # #include <glsl/fs.sc.bin.h>
        # #include <essl/fs.sc.bin.h>
        # #include <spirv/fs.sc.bin.h>
        # #if defined(_WIN32)
        # #include <dx11/vs.sc.bin.h>
        # #include <dx11/fs.sc.bin.h>
        # #endif //  defined(_WIN32)
        # #if __APPLE__
        # #include <metal/vs.sc.bin.h>
        # #include <metal/fs.sc.bin.h>
        # #endif // __APPLE__
        # #endif // SHADERS_H_big2_fs_ocornut_imgui
        # -- END NOTE
        
        string(REGEX REPLACE "[^a-zA-Z0-9_]" "_" MACRO_NAME "${NAMESPACE}_${OUTPUT_FILENAME}")

        string(APPEND INCLUDE_ALL_HEADER "#ifndef SHADERS_H_${MACRO_NAME}\n")
        string(APPEND INCLUDE_ALL_HEADER "#define SHADERS_H_${MACRO_NAME}\n")
        string(APPEND INCLUDE_ALL_HEADER "#include <generated/shaders/${NAMESPACE}/glsl/${OUTPUT_FILENAME}>\n")
        string(APPEND INCLUDE_ALL_HEADER "#include <generated/shaders/${NAMESPACE}/essl/${OUTPUT_FILENAME}>\n")
        string(APPEND INCLUDE_ALL_HEADER "#include <generated/shaders/${NAMESPACE}/spirv/${OUTPUT_FILENAME}>\n")
        string(APPEND INCLUDE_ALL_HEADER "#if defined(_WIN32)\n")
        string(APPEND INCLUDE_ALL_HEADER "#include <generated/shaders/${NAMESPACE}/dx11/${OUTPUT_FILENAME}>\n")
        string(APPEND INCLUDE_ALL_HEADER "#endif //  defined(_WIN32)\n")
        string(APPEND INCLUDE_ALL_HEADER "#if __APPLE__\n")
        string(APPEND INCLUDE_ALL_HEADER "#include <generated/shaders/${NAMESPACE}/metal/${OUTPUT_FILENAME}>\n")
        string(APPEND INCLUDE_ALL_HEADER "#endif // __APPLE__\n")
        string(APPEND INCLUDE_ALL_HEADER "#endif // SHADERS_H_${MACRO_NAME}\n")

    endforeach()
    file(WRITE "${SHADERS_OUT_DIR}/all.h" "${INCLUDE_ALL_HEADER}")
    list(APPEND OUTPUT_FILES "${SHADERS_OUT_DIR}/all.h")

    string(MD5 DIR_HASH "${SHADERS_DIR}")
    set(TARGET_NAME "Shaders_${DIR_HASH}")
    add_custom_target("${DIR_HASH}" ALL
            DEPENDS ${OUTPUT_FILES}
            SOURCES ${VARYING_DEF_LOCATION} ${FRAGMENT_SHADER_FILES} ${VERTEX_SHADER_FILES})

    add_library("${TARGET_NAME}" INTERFACE)
    add_dependencies("${TARGET_NAME}" shaderc "${DIR_HASH}")
    target_include_directories("${TARGET_NAME}" INTERFACE "${CMAKE_CURRENT_BINARY_DIR}/include")

    set("${TARGET_OUT_VAR}" "${TARGET_NAME}" PARENT_SCOPE)
endfunction()
