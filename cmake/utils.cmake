function(add_shaders_directory SHADERS_DIR TARGET_OUT_VAR)
    get_filename_component(SHADERS_DIR "${SHADERS_DIR}" ABSOLUTE)
    get_filename_component(NAMESPACE "${CMAKE_CURRENT_SOURCE_DIR}" NAME_WE)

    if(NOT EXISTS "${SHADERS_DIR}")
        message(NOTICE "Shaders directory does not exist")
        return()
    endif()

    file(GLOB VERTEX_SHADER_FILES CONFIGURE_DEPENDS FOLLOW_SYMLINKS "${SHADERS_DIR}/vs_*[!.def].sc")
    file(GLOB FRAGMENT_SHADER_FILES CONFIGURE_DEPENDS FOLLOW_SYMLINKS "${SHADERS_DIR}/fs_*[!.def].sc")

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

    bgfx_compile_shader_to_header(
            TYPE VERTEX
            SHADERS ${VERTEX_SHADER_FILES}
            VARYING_DEF "${VARYING_DEF_LOCATION}"
            OUTPUT_DIR "${SHADERS_OUT_DIR}"
            OUT_FILES_VAR VERTEX_OUTPUT_FILES
            INCLUDE_DIRS "${SHADERS_DIR}" "${BGFX_DIR}/src"
    )

    bgfx_compile_shader_to_header(
            TYPE FRAGMENT
            SHADERS ${FRAGMENT_SHADER_FILES}
            VARYING_DEF "${VARYING_DEF_LOCATION}"
            OUTPUT_DIR "${SHADERS_OUT_DIR}"
            OUT_FILES_VAR FRAGMENT_OUTPUT_FILES
            INCLUDE_DIRS "${SHADERS_DIR}" "${BGFX_DIR}/src"
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
        string(APPEND INCLUDE_ALL_HEADER "#include <generated/shaders/${NAMESPACE}/${OUTPUT_FILENAME}>\n")
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
