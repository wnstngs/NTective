/*!
 *  @file       imguimgr.hpp
 *  @brief      Dear ImGui manager.
 */

#pragma once

#include "../common/win32.h"

namespace Ui {

enum class STYLE_COLORS_THEME {
    Light,
    Dark
};

class IMGUI_MGR {
public:
    IMGUI_MGR(
        HWND WindowHandle,
        STYLE_COLORS_THEME Theme = STYLE_COLORS_THEME::Dark
    );

    ~IMGUI_MGR();
};

}
