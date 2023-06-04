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

function(json_config_dependencies)
    include(FetchContent)

    set(SINGLE_OPTIONS ROOT)
    set(ONE_VALUE_ARGS FILE)
    set(MULTI_VALUE_ARGS MEMBER_SELECTOR)
    cmake_parse_arguments(JSON_CONFIG "${SINGLE_OPTIONS}" "${ONE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}" ${ARGN})

    set_property(
            DIRECTORY
            APPEND
            PROPERTY CMAKE_CONFIGURE_DEPENDS JSON_CONFIG_FILE
    )

    set(DEPENDENCY_COMMON_OPTIONS)
    list(APPEND DEPENDENCY_COMMON_OPTIONS GIT_SHALLOW TRUE)
    list(APPEND DEPENDENCY_COMMON_OPTIONS GIT_REMOTE_UPDATE_STRATEGY CHECKOUT)
    list(APPEND DEPENDENCY_COMMON_OPTIONS GIT_SUBMODULES_RECURSE TRUE)

    if(JSON_CONFIG_ROOT)
        set(JSON_CONFIG_MEMBER_SELECTOR "")
    endif()

    file(READ ${JSON_CONFIG_FILE} CONFIG_FILE)
    string(JSON DEPENDENCIES_COUNT LENGTH "${CONFIG_FILE}" ${JSON_CONFIG_MEMBER_SELECTOR})
    math(EXPR DEPENDENCIES_RANGE_END "${DEPENDENCIES_COUNT} - 1")

    foreach (INDEX RANGE 0 "${DEPENDENCIES_RANGE_END}")
        string(JSON DEPENDENCY_JSON GET "${CONFIG_FILE}" ${JSON_CONFIG_MEMBER_SELECTOR} "${INDEX}")
        string(JSON DEPENDENCY_NAME GET "${DEPENDENCY_JSON}" "name")
        string(JSON DEPENDENCY_URL GET "${DEPENDENCY_JSON}" "url")
        string(JSON DEPENDENCY_VERSION GET "${DEPENDENCY_JSON}" "version")
        string(JSON DEPENDENCY_OPTIONS ERROR_VARIABLE OPTIONS_MISSING GET "${DEPENDENCY_JSON}" "options")

        string(CONFIGURE ${DEPENDENCY_VERSION} DEPENDENCY_VERSION)
        message(STATUS "Processing dependency: ${DEPENDENCY_NAME} - " ${DEPENDENCY_VERSION})

        fetchcontent_declare(
                "${DEPENDENCY_NAME}"
                GIT_REPOSITORY "${DEPENDENCY_URL}"
                GIT_TAG "${DEPENDENCY_VERSION}"
                ${DEPENDENCY_COMMON_OPTIONS}
        )

        if(NOT "${DEPENDENCY_OPTIONS}" MATCHES "NOTFOUND$")
            string(JSON OPTIONS_COUNT LENGTH "${DEPENDENCY_OPTIONS}")
            math(EXPR OPTIONS_RANGE_END "${OPTIONS_COUNT} - 1")

            foreach (OPTION_INDEX RANGE 0 ${OPTIONS_RANGE_END})
                string(JSON OPTION_INFO GET "${DEPENDENCY_OPTIONS}" "${INDEX}")
                string(JSON OPTION_NAME GET "${OPTION_INFO}" "name")
                string(JSON OPTION_VALUE GET "${OPTION_INFO}" "value")

                set(${OPTION_NAME}  ${OPTION_VALUE} CACHE INTERNAL "")
            endforeach ()
        endif()

        fetchcontent_makeavailable("${DEPENDENCY_NAME}")
    endforeach ()
endfunction()

