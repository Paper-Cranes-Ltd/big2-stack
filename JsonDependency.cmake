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
