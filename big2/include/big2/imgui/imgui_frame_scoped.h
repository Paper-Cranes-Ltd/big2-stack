//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//

#ifndef BIG2_STACK_IMGUI_FRAME_SCOPED_H_
#define BIG2_STACK_IMGUI_FRAME_SCOPED_H_

namespace big2 {

/**
 * @brief Will call ImGuiStartFrame() and ImGuiEndFrame() in a scope
 * @details Could be used with some helper macros in this project.
 * @see BIG2_SCOPE(assignment)
 */
class ImGuiFrameScoped final {
 public:
  ImGuiFrameScoped();
  ~ImGuiFrameScoped();
};

}

#endif //BIG2_STACK_IMGUI_FRAME_SCOPED_H_
