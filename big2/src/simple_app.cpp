//
// Copyright (c) 2024 Paper Cranes Ltd.
// All rights reserved.
//
#include <big2/simple_app.h>
#include <big2/default_quit_condition_app_extension.h>

#if BIG2_IMGUI_ENABLED
#include <big2/imgui/imgui_app_extension.h>
#endif

namespace big2 {
App CreateSimpleApp() {
  App app;
  app.AddExtension<big2::DefaultQuitConditionAppExtension>();
#if BIG2_IMGUI_ENABLED
  app.AddExtension<big2::ImGuiAppExtension>();
#endif // BIG2_IMGUI_ENABLED
  return app;
}
}
