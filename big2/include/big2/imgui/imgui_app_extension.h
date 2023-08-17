//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_IMGUI_APP_EXTENSION_H_
#define BIG2_STACK_IMGUI_APP_EXTENSION_H_

#if BIG2_IMGUI_ENABLED

#include <vector>
#include <big2/app_extension_base.h>
#include <big2/imgui/imgui_context_wrapper.h>

namespace big2 {

class ImGuiAppExtension final : public AppExtensionBase {

 protected:
  void OnWindowCreated(Window& window) override;
  void OnWindowDestroyed(Window& window) override;
  void OnRender(Window& window) override;

 private:
  std::vector<ImGuiContextWrapper> contexts_;
};

}

#endif // BIG2_IMGUI_ENABLED

#endif //BIG2_STACK_IMGUI_APP_EXTENSION_H_
