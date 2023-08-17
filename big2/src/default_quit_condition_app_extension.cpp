//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//
#include <big2/default_quit_condition_app_extension.h>
#include <big2/app.h>

namespace big2 {

void DefaultQuitConditionAppExtension::OnUpdate(std::float_t dt) {
  AppExtensionBase::OnUpdate(dt);
  if (app_->GetWindows().empty()) {
    app_->Stop();
  }
}

}
