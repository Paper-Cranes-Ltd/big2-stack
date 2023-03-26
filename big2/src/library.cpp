#include <big2.h>
#include <backends/imgui_impl_glfw.h>
#include <imgui_impl_bgfx.h>

namespace big2 {

#if BIG2_IMGUI_ENABLED
void ImGuiInit(GLFWwindow *window, bgfx::ViewId view_id) {
  ImGui_ImplGlfw_InitForOther(window, true);
  ImGui_ImplBgfx_Init(view_id);
}

void ImGuiTerminate() {
  ImGui_ImplBgfx_Shutdown();
  ImGui_ImplGlfw_Shutdown();
}

void ImGuiBeginFrame() {
  ImGui_ImplBgfx_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void ImGuiEndFrame() {
  ImGui::Render();
  ImGui_ImplBgfx_RenderDrawData(ImGui::GetDrawData());
}
#endif // BIG2_IMGUI_ENABLED

}