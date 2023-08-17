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

/**
 * @brief Stops the app when there are no open windows.
 */
class DefaultQuitConditionAppExtension final : public AppExtensionBase {
 protected:
  void OnUpdate(std::float_t dt) override;
};

}

#endif //BIG2_STACK_DEFAULT_QUIT_CONDITION_APP_EXTENSION_H_
