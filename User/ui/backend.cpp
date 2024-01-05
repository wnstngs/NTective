#include "backend.hpp"
#include "../common/log.hpp"

namespace Ui {

GFX_BACKEND::GFX_BACKEND(
    HWND WindowHandle
) : WindowHandle_(WindowHandle)
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc{};
    swapChainDesc.BufferDesc.Width = 0;
    swapChainDesc.BufferDesc.Height = 0;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = WindowHandle_;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = 0;

    D3D11CreateDeviceAndSwapChain(nullptr,
                                  D3D_DRIVER_TYPE_HARDWARE,
                                  nullptr,
                                  0,
                                  nullptr,
                                  0,
                                  D3D11_SDK_VERSION,
                                  &swapChainDesc,
                                  &SwapChain_,
                                  &Device_,
                                  nullptr,
                                  &DeviceContext_);

    ID3D11Resource *backBuffer = nullptr;
    SwapChain_->GetBuffer(DXGI_SWAP_EFFECT_DISCARD,
                          __uuidof(ID3D11Resource),
                          reinterpret_cast<void **>(&backBuffer));
    Device_->CreateRenderTargetView(backBuffer,
                                    nullptr,
                                    &RenderTargetView_);
    backBuffer->Release();
    backBuffer = nullptr;
}

GFX_BACKEND::~GFX_BACKEND()
{
    Cleanup();
}

void
GFX_BACKEND::EndScene()
{
    if (!SwapChain_) {
        LOG.Warning(L"SwapChain_ is null");
        return;
    }

    SwapChain_->Present(1, 0);
}

void
GFX_BACKEND::ClearBuffer(
    const float rgbaColor[4]
)
{
    if (!RenderTargetView_) {
        LOG.Warning(L"RenderTargetView_ is null");
        return;
    }

    if (!rgbaColor) {
        LOG.Warning(L"rgbaColor is null");
        return;
    }

    DeviceContext_->ClearRenderTargetView(RenderTargetView_, rgbaColor);
}

void
GFX_BACKEND::Cleanup()
{
    if (RenderTargetView_) {
        RenderTargetView_->Release();
        RenderTargetView_ = nullptr;
    }
    if (DeviceContext_) {
        DeviceContext_->Release();
        DeviceContext_ = nullptr;
    }
    if (SwapChain_) {
        SwapChain_->Release();
        SwapChain_ = nullptr;
    }
    if (Device_) {
        Device_->Release();
        Device_ = nullptr;
    }
}

}
