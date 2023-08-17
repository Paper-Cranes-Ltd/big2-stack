//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_APP_H_
#define BIG2_STACK_APP_H_

#include <gsl/gsl>
#include <cstdint>
#include <vector>
#include <chrono>
#include <cmath>
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
  [[nodiscard]] std::float_t GetDeltaTime() const { return delta_time_; }

 private:

  using time_point = std::chrono::steady_clock::time_point;

  void UpdateClock();
  void ProcessClosedWindows();
  void MandatoryBeginFrame();

  std::vector<std::unique_ptr<AppExtensionBase>> extensions_;
  std::vector<Window> windows_;

  time_point previous_frame_time_;
  std::float_t delta_time_;
  bool is_running_ = false;

  GlfwInitializationScoped glfw_initialization_scoped_;
  BgfxInitializationScoped bgfx_initialization_scoped_;
};

}
#endif //BIG2_STACK_APP_H_
