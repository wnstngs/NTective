/*!
 *  @file       backend.hpp
 *  @brief      DirectX 11 graphics backend for UI.
 */

#pragma once

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include "../common/win32.h"

namespace Ui {

class GFX_BACKEND {
public:
    GFX_BACKEND(
        HWND WindowHandle
    );

    ~GFX_BACKEND();

    void
    EndScene();

    void
    ClearBuffer(
        const float rgbaColor[4]
    );

    void
    InitImgui();

private:
    void
    Cleanup();

    HWND WindowHandle_ = nullptr;
    ID3D11Device *Device_ = nullptr;
    IDXGISwapChain *SwapChain_ = nullptr;
    ID3D11DeviceContext *DeviceContext_ = nullptr;
    ID3D11RenderTargetView *RenderTargetView_ = nullptr;
};

}
