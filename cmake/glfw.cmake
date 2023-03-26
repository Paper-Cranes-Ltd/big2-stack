fetchcontent_declare(
        glfw
        GIT_REPOSITORY "https://github.com/glfw/glfw.git"
        GIT_TAG "3.3.8"
        ${BIG2_COMMON_DEPS_OPTIONS}
)

set(GLFW_BUILD_EXAMPLES  OFF CACHE INTERNAL "")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "")
set(GLFW_INSTALL OFF CACHE INTERNAL "")
fetchcontent_makeavailable(glfw)