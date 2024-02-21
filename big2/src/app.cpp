//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//
#include <big2/app.h>
#include <GLFW/glfw3.h>
#include <execution>
#include <big2/app_extension_base.h>
#include <big2/glfw/glfw_utils.h>
#include <big2/event_queue.h>
#include <big2/bgfx/bgfx_utils.h>
#include <big2/execution.h>
#include <chrono>

namespace big2 {
Window &App::CreateWindow(const std::string &title, glm::ivec2 size) {
  Expects(windows_.empty() || BgfxSupportsMultipleWindows());

  glfwWindowHint(GLFW_FLOATING, false);
  Window window(title.c_str(), size);
  window.SetIsScoped(false);
  big2::GlfwEventQueue::ConnectWindow(window);

  auto call_extensions_window_created = [&window](std::unique_ptr<AppExtensionBase> &extension) {
    extension->OnWindowCreated(window);
  };

  std::for_each(EXECUTION_POLICY(std::execution::seq) extensions_.begin(), extensions_.end(), call_extensions_window_created);
  return windows_.emplace_back(window);
}

void App::Run() {
  state_ = ActiveState::Run;

  std::for_each(EXECUTION_POLICY(std::execution::seq)
                extensions_.begin(),
                extensions_.end(),
                [this](std::unique_ptr<AppExtensionBase> &extension) {
                  extension->Initialize(this);
                });

  auto call_extensions_frame_begin = [](std::unique_ptr<AppExtensionBase> &extension) {
    extension->OnFrameBegin();
  };

  auto call_extensions_frame_end = [](std::unique_ptr<AppExtensionBase> &extension) {
    extension->OnFrameEnd();
  };

  auto call_extensions_update = [this](std::unique_ptr<AppExtensionBase> &extension) {
    extension->OnUpdate(delta_time_);
  };

  auto call_extensions_window_render = [this](std::unique_ptr<AppExtensionBase> &extension) {
    for (Window &window : windows_) {
      bgfx::touch(window.GetView());
      extension->OnRender(window);
    }
  };

  while (state_ != ActiveState::Stop) {
    MandatoryBeginFrame();

    std::for_each(EXECUTION_POLICY(std::execution::seq) extensions_.begin(), extensions_.end(), call_extensions_frame_begin);

    if (state_ != ActiveState::Pause) {
      std::for_each(EXECUTION_POLICY(std::execution::seq) extensions_.begin(), extensions_.end(), call_extensions_update);
    }

    std::for_each(EXECUTION_POLICY(std::execution::seq) extensions_.begin(), extensions_.end(), call_extensions_window_render);
    std::for_each(EXECUTION_POLICY(std::execution::seq) extensions_.begin(), extensions_.end(), call_extensions_frame_end);

    bgfx::frame();
    ProcessClosedWindows();
  }

  // Terminate
  std::for_each(EXECUTION_POLICY(std::execution::seq)
                extensions_.begin(),
                extensions_.end(),
                [](std::unique_ptr<AppExtensionBase> &extension) {
                  extension->OnTerminate();
                });
}

void App::MandatoryBeginFrame() {
  UpdateDeltaTime();
  GlfwEventQueue::PollEvents();

  for (Window &window : windows_) {
    if (big2::GlfwEventQueue::HasEventType<GlfwEvent::FrameBufferResized>(window)) {
      const glm::ivec2 window_resolution = GetWindowResolution(window);
      window.SetFrameSize(window_resolution);
    }
  }
}

void App::ProcessClosedWindows() {
  for (Window &window : windows_) {
    if (!window.GetShouldClose()) {
      continue;
    }

    auto call_window_destroy = [&window](std::unique_ptr<AppExtensionBase> &extension) {
      extension->OnWindowDestroyed(window);
    };

    window.Dispose();
    std::for_each(extensions_.begin(), extensions_.end(), call_window_destroy);
  }

  std::erase_if(windows_, [](Window &window) { return window.GetShouldClose(); });
}

App::App(bgfx::RendererType::Enum renderer_type) {
  glfw_initialization_scoped_ = std::make_unique<GlfwInitializationScoped>();
  bgfx_initialization_scoped_ = std::make_unique<BgfxInitializationScoped>(renderer_type);
  big2::GlfwEventQueue::Initialize();
}

void App::UpdateDeltaTime() {
  using float_duration_seconds = std::chrono::duration<float, std::chrono::seconds::period>;

  time_point current_time = std::chrono::steady_clock::now();
  delta_time_ = float_duration_seconds(current_time - previous_frame_time_).count();
  previous_frame_time_ = current_time;
}
}
