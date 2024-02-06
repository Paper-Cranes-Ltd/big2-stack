//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#include <big2.h>

#if BIG2_IMGUI_ENABLED
#include <imgui.h>
#endif // BIG2_IMGUI_ENABLED

#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <gsl/gsl>
#include <big2/simple_app.h>

class MainAppExtension final : public big2::AppExtensionBase {
  protected:

    void OnRender(big2::Window &window) override {
      AppExtensionBase::OnRender(window);

#if BIG2_IMGUI_ENABLED
      BIG2_SCOPE_VAR(big2::ImGuiFrameScoped) {
        ImGui::ShowDemoWindow();
      }
#endif // BIG2_IMGUI_ENABLED
    }
};

int main(std::int32_t, gsl::zstring []) {
  big2::App app = big2::CreateSimpleApp();
  app.AddExtension<MainAppExtension>();

  app.CreateWindow("My App", {800, 600});
  app.Run();

  return 0;
}
