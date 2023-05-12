fetchcontent_declare(
        bgfx
        GIT_REPOSITORY "https://github.com/bkaradzic/bgfx.cmake.git"
        GIT_TAG "v1.118.8455-425"
        ${BIG2_COMMON_DEPS_OPTIONS}
)

set(BGFX_BUILD_TOOLS ON CACHE INTERNAL "")
set(BGFX_BUILD_EXAMPLES OFF CACHE INTERNAL "")
set(BGFX_CUSTOM_TARGETS OFF CACHE INTERNAL "")

fetchcontent_makeavailable(bgfx)
