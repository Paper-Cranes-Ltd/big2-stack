//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//
#include <big2/app_default_quit_condition_extension.h>
#include <big2/app.h>

namespace big2 {

void AppDefaultQuitConditionExtension::OnWindowCreated(Window &window) {
  AppExtensionBase::OnWindowCreated(window);
  windows_count_++;
}

void AppDefaultQuitConditionExtension::OnWindowDestroyed(Window &window) {
  AppExtensionBase::OnWindowDestroyed(window);
  windows_count_--;
  if(windows_count_ == 0) {
    app_->SetIsRunning(false);
  }
}

}
