fetchcontent_declare(
    spdlog
    GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
    GIT_TAG "v1.11.0"
    ${BIG2_COMMON_DEPS_OPTIONS}
)
fetchcontent_makeavailable(spdlog)
