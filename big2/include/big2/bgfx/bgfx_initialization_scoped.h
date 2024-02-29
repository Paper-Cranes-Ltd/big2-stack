//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_BGFX_INITIALIZATION_SCOPED_H_
#define BIG2_STACK_BGFX_INITIALIZATION_SCOPED_H_

#include <bgfx/bgfx.h>
#include <gsl/gsl>
#include <glm/glm.hpp>

struct GLFWwindow;

namespace big2 {
/**
 * @brief A scoped singleton initialization class for BGFX.
 * @details This class will crashes if terminated or initialized twice and will also set
 * a default error callback that will log any errors.
 * @see BIG2_SCOPE(assignment)
 */
class BgfxInitializationScoped final {
  public:
    BgfxInitializationScoped(bgfx::RendererType::Enum renderer_type = bgfx::RendererType::Count, std::uint64_t capabilities = std::numeric_limits<std::uint64_t>::max());

    BgfxInitializationScoped(BgfxInitializationScoped && other);

    BgfxInitializationScoped &operator=(BgfxInitializationScoped && other) noexcept;

    BgfxInitializationScoped(const BgfxInitializationScoped &) = delete;

    BgfxInitializationScoped &operator=(const BgfxInitializationScoped &) = delete;

    ~BgfxInitializationScoped();

    static BgfxInitializationScoped *GetInstance() { return instance_; }

    /**
     * \brief To be called after window is created if you initially created the instance in headless mode.
     */
    void ReInitialize(gsl::not_null<GLFWwindow *> window, glm::ivec2 size);

  private:
    static BgfxInitializationScoped *instance_;
    bgfx::RendererType::Enum renderer_type_ = bgfx::RendererType::Count;
    std::uint64_t capabilities_ = std::numeric_limits<std::uint64_t>::max();
};
}

#endif //BIG2_STACK_BGFX_INITIALIZATION_SCOPED_H_
