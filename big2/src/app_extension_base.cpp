//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//
#include <big2/app_extension_base.h>

namespace big2 {

void AppExtensionBase::Initialize(App *app) {
  app_ = app;
  OnInitialize();
}

}
