#include <big2.h>

#include <GLFW/glfw3.h>

#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3native.h>

#if BIG2_IMGUI_ENABLED

#include <backends/imgui_impl_glfw.h>
#include <big2/imgui_impl_bgfx.h>

#endif // BIG2_IMGUI_ENABLED

namespace big2
{

#if BIG2_IMGUI_ENABLED
void ImGuiInit(GLFWwindow *window, bgfx::ViewId view_id)
{
    ImGui_ImplGlfw_InitForOther(window, true);
    ImGui_ImplBgfx_Init(view_id);
}

void ImGuiTerminate()
{
    ImGui_ImplBgfx_Shutdown();
    ImGui_ImplGlfw_Shutdown();
}

void ImGuiBeginFrame()
{
    ImGui_ImplBgfx_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiEndFrame()
{
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplBgfx_RenderDrawData(ImGui::GetDrawData());
}
#endif // BIG2_IMGUI_ENABLED

void SetNativeWindowData(bgfx::Init &init_obj, GLFWwindow *window)
{
#if BX_PLATFORM_LINUX
    init_obj.platformData.ndt = glfwGetX11Display();
    init_obj.platformData.nwh = reinterpret_cast<void *>(glfwGetX11Window(window));
#elif BX_PLATFORM_OSX
    init_obj.platformData.nwh = glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
    init_obj.platformData.nwh = glfwGetWin32Window(window);
#endif
}

}
