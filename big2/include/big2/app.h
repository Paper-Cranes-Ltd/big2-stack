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

#include "app_extension_base.h"

namespace big2 {
class AppExtensionBase;

template<class TExtension>
concept AppExtensionDerived = std::is_base_of_v<AppExtensionBase, TExtension>;

class App final {
 public:
  enum class ActiveState : std::uint8_t {
   Unset,
   Run,
   Pause,
   Stop,
  };

  explicit App(bgfx::RendererType::Enum renderer_type = bgfx::RendererType::Count);

  /**
   * @brief Creates an extension in the app.
   * @note You can only add extensions before running the app
   * @tparam TExtension A class that inherits AppExtensionBase
   * @see AppExtensionBase
   */
  template<AppExtensionDerived TExtension>
  App &AddExtension() {
   extensions_.push_back(std::make_unique<TExtension>());

   if (state_ == ActiveState::Run || state_ == ActiveState::Pause) {
    extensions_.back()->Initialize(this);
   }

   return *this;
  }

  /**
   * @brief Runs the application.
   * @details This is blocking to the current thread and will return only after the application is stopped.
   */
  void Run();

  /**
   * @brief Stops the application on the next frame
   */
  void Stop() { SetActiveState(ActiveState::Stop); }

  /**
   * @brief Pauses the application. Updates on extensions will not be called.
   */
  void Pause() { SetActiveState(ActiveState::Pause); }

  /**
   * @brief Checks if the application is still in a running state.
   */
  [[nodiscard]] bool GetIsRunning() const { return state_ == ActiveState::Run; }

  /**
   * @brief Creates a window with the given title and size.
   */
  Window &CreateWindow(const std::string &title, glm::ivec2 size);

  /**
   * @brief Gets all created windows that are linked to the app.
   */
  [[nodiscard]] const std::vector<Window> &GetWindows() const { return windows_; }

  /**
   * @copydoc GetWindows() const
   */
  [[nodiscard]] std::vector<Window> &GetWindows() { return windows_; }

  /**
   * @brief Gets the delta time for the current frame.
   * @return The delta time is a real number representing seconds.
   */
  [[nodiscard]] std::float_t GetDeltaTime() const { return delta_time_; }

 private:
  using time_point = std::chrono::steady_clock::time_point;

  void UpdateDeltaTime();
  void ProcessClosedWindows();
  void MandatoryBeginFrame();

  [[nodiscard]] ActiveState GetActiveState() const { return state_; }
  void SetActiveState(ActiveState value) { state_ = value; }

  std::vector<std::unique_ptr<AppExtensionBase> > extensions_;
  std::vector<Window> windows_;

  time_point previous_frame_time_;
  std::float_t delta_time_ = 0.0f;
  ActiveState state_ = ActiveState::Unset;

  std::unique_ptr<GlfwInitializationScoped> glfw_initialization_scoped_ = nullptr;
  std::unique_ptr<BgfxInitializationScoped> bgfx_initialization_scoped_ = nullptr;
};
}
#endif //BIG2_STACK_APP_H_
