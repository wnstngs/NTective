/*!
 *  @file       imguimgr.cpp
 *  @brief      Dear ImGui manager.
 */

#include "imguimgr.hpp"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

namespace Ui {

IMGUI_MGR::IMGUI_MGR(
    HWND WindowHandle,
    const STYLE_COLORS_THEME Theme
)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    if (Theme == STYLE_COLORS_THEME::Light) {
        ImGui::StyleColorsLight();
    } else if (Theme == STYLE_COLORS_THEME::Dark) {
        ImGui::StyleColorsDark();
    }

    ImGuiIO &imguiIo = ImGui::GetIO();
    imguiIo.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
}

IMGUI_MGR::~IMGUI_MGR()
{
    ImGui::DestroyContext();
}

}
