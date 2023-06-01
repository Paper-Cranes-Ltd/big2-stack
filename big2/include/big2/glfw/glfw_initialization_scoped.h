//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_GLFW_INITIALIZATION_SCOPED_H_
#define BIG2_STACK_GLFW_INITIALIZATION_SCOPED_H_

namespace big2 {

/**
 * @brief A scoped initialization class for GLFW that will also terminate it.
 * @details This class will warn if terminated or initialized twice and will also set
 * a default error callback that will log any errors.
 * @see BIG2_SCOPE(assignment)
 */
class GlfwInitializationScoped final {
 public:
  GlfwInitializationScoped();

  GlfwInitializationScoped(GlfwInitializationScoped &&) = default;

  GlfwInitializationScoped &operator=(GlfwInitializationScoped &&) = default;

  GlfwInitializationScoped(const GlfwInitializationScoped &) = delete;

  GlfwInitializationScoped &operator=(const GlfwInitializationScoped &) = delete;

  ~GlfwInitializationScoped();

 private:
  static inline bool is_initialized_ = false;
};

}

#endif //BIG2_STACK_GLFW_INITIALIZATION_SCOPED_H_
