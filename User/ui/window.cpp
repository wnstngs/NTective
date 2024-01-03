/*!
 *  @file       window.cpp
 *  @brief      Core window management.
 */

#include "window.hpp"
#include "../common/log.hpp"
#include "../common/strutil.hpp"

#include <exception>

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
    } catch (...) {
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
