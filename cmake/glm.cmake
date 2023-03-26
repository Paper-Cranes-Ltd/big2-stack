fetchcontent_declare(
        glm
        GIT_REPOSITORY "https://github.com/g-truc/glm.git"
        GIT_TAG "0.9.9.8"
        ${BIG2_COMMON_DEPS_OPTIONS}
)
fetchcontent_makeavailable(glm)