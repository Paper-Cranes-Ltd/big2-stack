

function(add_shaders_directory SHADERS_DIR TARGET_OUT_VAR)
    get_filename_component(SHADERS_DIR "${SHADERS_DIR}" ABSOLUTE)
    get_filename_component(NAMESPACE "${CMAKE_CURRENT_SOURCE_DIR}" NAME_WE)

    if(NOT EXISTS "${SHADERS_DIR}")
        message(NOTICE "Shaders directory does not exist")
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

    # 120|300_es|spirv|metal|s_3_0|s_4_0|s_5_0
    set(PROFILES 120 300_es spirv s_3_0 s_4_0 s_5_0)
    if(APPLE)
        list(APPEND PROFILES metal)
    endif()

    file(GLOB VERTEX_SHADER_FILES CONFIGURE_DEPENDS FOLLOW_SYMLINKS "${SHADERS_DIR}/vs_*[!.def].sc")
    bgfx_compile_shader_to_header(
            TYPE VERTEX
            SHADERS ${VERTEX_SHADER_FILES}
            VARYING_DEF "${VARYING_DEF_LOCATION}"
            OUTPUT_DIR "${SHADERS_OUT_DIR}"
            OUT_FILES_VAR VERTEX_OUTPUT_FILES
            PROFILES ${PROFILES}
            INCLUDE_DIRS "${SHADERS_DIR}" "${BGFX_DIR}/src"
    )

    file(GLOB FRAGMENT_SHADER_FILES CONFIGURE_DEPENDS FOLLOW_SYMLINKS "${SHADERS_DIR}/fs_*[!.def].sc")
    bgfx_compile_shader_to_header(
            TYPE FRAGMENT
            SHADERS ${FRAGMENT_SHADER_FILES}
            VARYING_DEF "${VARYING_DEF_LOCATION}"
            OUTPUT_DIR "${SHADERS_OUT_DIR}"
            OUT_FILES_VAR FRAGMENT_OUTPUT_FILES
            PROFILES ${PROFILES}
            INCLUDE_DIRS "${SHADERS_DIR}" "${BGFX_DIR}/src"
    )
    
    set(OUTPUT_FILES)
    list(APPEND OUTPUT_FILES ${VERTEX_OUTPUT_FILES})
    list(APPEND OUTPUT_FILES ${FRAGMENT_OUTPUT_FILES})

    message(STATUS ${OUTPUT_FILES})

    list(LENGTH OUTPUT_FILES SHADER_COUNT)
    if(SHADER_COUNT EQUAL 0)
        return()
    endif()
    file(TOUCH ${OUTPUT_FILES})
    
    set(INCLUDE_ALL_HEADER "")
    foreach(OUTPUT_FILE IN LISTS OUTPUT_FILES)
        get_filename_component(OUTPUT_FILENAME "${OUTPUT_FILE}" NAME)
        string(APPEND INCLUDE_ALL_HEADER "#include <generated/shaders/${NAMESPACE}/${OUTPUT_FILENAME}>\n")
    endforeach()
    file(WRITE "${SHADERS_OUT_DIR}/all.h" "${INCLUDE_ALL_HEADER}")
    list(APPEND OUTPUT_FILES "${SHADERS_OUT_DIR}/all.h")
    
    string(MD5 DIR_HASH "${SHADERS_DIR}")
    set(TARGET_NAME "Shaders_${DIR_HASH}")
    add_custom_target("${DIR_HASH}" ALL SOURCES ${OUTPUT_FILES})

    add_library("${TARGET_NAME}" INTERFACE ${OUTPUT_FILES})
    add_dependencies("${TARGET_NAME}" shaderc "${DIR_HASH}")
    target_include_directories("${TARGET_NAME}" INTERFACE "${CMAKE_CURRENT_BINARY_DIR}/include")

    set("${TARGET_OUT_VAR}" "${TARGET_NAME}" PARENT_SCOPE)
endfunction()

