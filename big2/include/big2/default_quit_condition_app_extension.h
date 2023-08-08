//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_DEFAULT_QUIT_CONDITION_APP_EXTENSION_H_
#define BIG2_STACK_DEFAULT_QUIT_CONDITION_APP_EXTENSION_H_

#include <vector>
#include <big2/app_extension_base.h>
#include <big2/imgui/imgui_context_wrapper.h>

namespace big2 {

class DefaultQuitConditionAppExtension final : public AppExtensionBase {

 protected:
  void OnWindowCreated(Window &window) override;
  void OnWindowDestroyed(Window &window) override;
 private:
  std::uint32_t windows_count_ = 0;
};

}

#endif //BIG2_STACK_DEFAULT_QUIT_CONDITION_APP_EXTENSION_H_
