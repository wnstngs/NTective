/*!
 *  @file       winclass.cpp
 *  @brief      Window class management.
 */

#include "winclass.hpp"
#include "winbase.hpp"
#include "../common/log.hpp"

namespace Ui {

LRESULT
WINDOW_CLASS_BASE::ForwardMessage(
    WINDOW_BASE *Window,
    HWND Handle,
    UINT Message,
    WPARAM WParam,
    LPARAM LParam
)
{
    return Window->HandleMessage(Handle, Message, WParam, LParam);
}

WINDOW_CLASS::WINDOW_CLASS(
    const std::wstring &ClassName
)
    : Instance_(GetModuleHandle(nullptr))
{
    const WNDCLASSEXW windowClass{
        .cbSize = sizeof(windowClass),
        .style = CS_OWNDC,
        .lpfnWndProc = &WINDOW_CLASS::HandleMessageSetup,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = Instance_,
        .hIcon = nullptr,
        .hCursor = nullptr,
        .hbrBackground = nullptr,
        .lpszMenuName = nullptr,
        .lpszClassName = ClassName.c_str(),
        .hIconSm = nullptr,
    };

    Atom_ = RegisterClassExW(&windowClass);

    if (!Atom_) {
        LOG.Error(L"Failed to register window class");
        throw std::runtime_error("Failed to register window class");
    }
}

WINDOW_CLASS::~WINDOW_CLASS()
{
    if (!UnregisterClass(MAKEINTATOM(Atom_), Instance_)) {
        LOG.Warning(L"Failed to unregister window class");
    }
}

ATOM
WINDOW_CLASS::GetAtom()
{
    return Atom_;
}

HINSTANCE
WINDOW_CLASS::GetInstance()
{
    return Instance_;
}

LRESULT
WINDOW_CLASS::HandleMessageSetup(
    HWND Handle,
    UINT Message,
    WPARAM WParam,
    LPARAM LParam
)
{
    if (Message == WM_NCCREATE) {
        const CREATESTRUCTW *const createStruct = reinterpret_cast<CREATESTRUCTW *>(LParam);
        const auto mainWindow = static_cast<WINDOW_BASE *>(createStruct->lpCreateParams);

        SetWindowLongPtrW(Handle,
                          GWLP_USERDATA,
                          reinterpret_cast<LONG_PTR>(mainWindow));
        if (auto lastError = GetLastError()) {
            LOG.Warning(L"SetWindowLongPtrW failed");
        }

        SetWindowLongPtrW(Handle,
                          GWLP_WNDPROC,
                          reinterpret_cast<LONG_PTR>(&WINDOW_CLASS::HandleMessageThunk));
        if (auto lastError = GetLastError()) {
            LOG.Warning(L"SetWindowLongPtrW failed");
        }

        return ForwardMessage(mainWindow,
                              Handle,
                              Message,
                              WParam,
                              LParam);
    }

    return DefWindowProc(Handle,
                         Message,
                         WParam,
                         LParam);
}


LRESULT
WINDOW_CLASS::HandleMessageThunk(
    HWND Handle,
    UINT Message,
    WPARAM WParam,
    LPARAM LParam
)
{
    const auto window =
        reinterpret_cast<WINDOW_BASE *>(GetWindowLongPtrW(Handle, GWLP_USERDATA));
    return ForwardMessage(window,
                          Handle,
                          Message,
                          WParam,
                          LParam);
}

}
