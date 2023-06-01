//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//
#include <big2/imgui/imgui_frame_scoped.h>
#include <big2/imgui/imgui.h>

namespace big2 {

ImGuiFrameScoped::ImGuiFrameScoped() {
  big2::ImGuiBeginFrame();
}

ImGuiFrameScoped::~ImGuiFrameScoped() {
  big2::ImGuiEndFrame();
}


}
