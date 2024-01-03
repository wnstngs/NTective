/*!
 *  @file       winimpl.cpp
 *  @brief      Implementation of the base window class.
 */

#include "winimpl.hpp"
#include "winclass.hpp"
#include "../common/log.hpp"
#include "../common/strutil.hpp"

namespace Ui {

MAIN_WINDOW::MAIN_WINDOW(
    std::shared_ptr<WINDOW_CLASS_BASE> WindowClass,
    const std::wstring &Title
)
    : WindowClass_(std::move(WindowClass))
{
    constexpr DWORD windowStyles = WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
    constexpr DWORD windowStylesEx = 0;
    const HMODULE moduleHandle = GetModuleHandleW(nullptr);

    Handle_ = CreateWindowExW(windowStylesEx,
                              MAKEINTATOM(WindowClass_->GetAtom()),
                              Title.c_str(),
                              windowStyles,
                              CW_USEDEFAULT,
                              CW_USEDEFAULT,
                              600,
                              400,
                              nullptr,
                              nullptr,
                              moduleHandle,
                              this);

    if (!Handle_) {
        LOG.Error(L"Failed to create window");
        throw std::runtime_error("Failed to create window");
    }
}

MAIN_WINDOW::~MAIN_WINDOW()
{
    if (!DestroyWindow(Handle_)) {
        LOG.Warning(L"Failed to destroy window");
    }
}

HWND
MAIN_WINDOW::GetHandle()
{
    return Handle_;
}

LRESULT
MAIN_WINDOW::HandleMessage(
    HWND Handle,
    UINT Message,
    WPARAM WParam,
    LPARAM LParam
)
{
    try {
        switch (Message) {
        case WM_DESTROY:
            Handle_ = nullptr;
            PostQuitMessage(0);
            return 0;
        case WM_CLOSE:
            return 0;
        default:
            return DefWindowProcW(Handle, Message, WParam, LParam);
        }
    } catch (const std::exception &e) {
        LOG.Error(std::format(L"An exception occurred: {}",
                              Common::Util::StringToWstring(e.what())));
    }
    catch (...) {
        LOG.Error(L"An unknown exception occurred");
    }
    return DefWindowProcW(Handle, Message, WParam, LParam);
}

void
MAIN_WINDOW::MessageLoop()
{
    MSG message{};
    while (GetMessageW(&message, Handle_, 0, 0)) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
}

}
