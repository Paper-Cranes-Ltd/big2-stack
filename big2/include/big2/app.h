//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_APP_H_
#define BIG2_STACK_APP_H_

#include <gsl/gsl>
#include <cstdint>
#include <vector>
#include <big2/window.h>
#include <big2/glfw/glfw_initialization_scoped.h>
#include <big2/bgfx/bgfx_view_scoped.h>
#include <big2/bgfx/bgfx_frame_buffer_scoped.h>
#include <big2/bgfx/bgfx_initialization_scoped.h>

namespace big2 {

class AppExtensionBase;

class App final {

 public:

  App();

  template<class TExtension>
  requires std::is_base_of_v<AppExtensionBase, TExtension>
  App &AddExtension() {
    extensions_.push_back(std::make_unique<TExtension>());
    return *this;
  }

  Window &CreateWindow(const std::string &title, glm::ivec2 size);
  void Run();

  [[nodiscard]] const std::vector<Window> &GetWindows() const { return windows_; }
  [[nodiscard]] std::vector<Window> &GetWindows() { return windows_; }

  bool GetIsRunning() const { return is_running_; }
  void SetIsRunning(bool value) { is_running_ = value; }

 private:

  std::vector<std::unique_ptr<AppExtensionBase>> extensions_;
  std::vector<Window> windows_;
  GlfwInitializationScoped glfw_initialization_scoped_;
  BgfxInitializationScoped bgfx_initialization_scoped_;
  bool is_running_ = false;
  void ProcessClosedWindows();
  void MandatoryBeginFrame();
};

}
#endif //BIG2_STACK_APP_H_
