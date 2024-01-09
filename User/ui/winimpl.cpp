/*!
 *  @file       winimpl.cpp
 *  @brief      Implementation of the base window class.
 */

#include "winimpl.hpp"

#include "imgui_impl_win32.h"
#include "winclass.hpp"
#include "../common/log.hpp"
#include "../common/strutil.hpp"

extern
IMGUI_IMPL_API
LRESULT
ImGui_ImplWin32_WndProcHandler(
    HWND hWnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
);

namespace Ui {

MAIN_WINDOW::MAIN_WINDOW(
    std::shared_ptr<WINDOW_CLASS_BASE> WindowClass,
    const std::wstring &Title
) : WindowClass_(std::move(WindowClass)),
    MessageLoopThread_{
        &MAIN_WINDOW::MessageLoop,
        this
    }
{
    auto future = JobQueue_.Enqueue([=, this] {
        constexpr DWORD windowStyles = WS_OVERLAPPEDWINDOW;
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
            throw WINDOW_EXCEPTION{"Failed to create window"};
        }

        ShowWindow(Handle_, SW_SHOWDEFAULT);
        UpdateWindow(Handle_);

        ImguiMgr_ = std::make_unique<IMGUI_MGR>(Handle_);

        ImGui_ImplWin32_Init(Handle_);

        GfxBackend_ = std::make_unique<GFX_BACKEND>(Handle_);
    });

    StartSignal_.release();
    future.get();
}

MAIN_WINDOW::~MAIN_WINDOW()
{
    Dispatch([this] {
        ImGui_ImplWin32_Shutdown();
        if (!DestroyWindow(Handle_)) {
            LOG.Warning(L"Failed to destroy window");
        }
    });
    MessageLoopThread_.join();
}

HWND
MAIN_WINDOW::GetHandle()
{
    return Handle_;
}

GFX_BACKEND &
MAIN_WINDOW::GetRenderer()
{
    return *GfxBackend_;
}

bool
MAIN_WINDOW::IsClosing()
{
    return IsClosing_;
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
        if (ImGui_ImplWin32_WndProcHandler(Handle,
                                           Message,
                                           WParam,
                                           LParam)) {
            return true;
        }

        switch (Message) {
        case WM_DESTROY: {
            Handle_ = nullptr;
            PostQuitMessage(0);
            return 0;
        }
        case WM_CLOSE: {
            IsClosing_ = true;
            return 0;
        }
        case WM_JOB: {
            JobQueue_.PopAndExecute();
            return 0;
        }
        case WM_SIZE: {
            if (WParam == SIZE_MINIMIZED) {
                return 0;
            }
            ResizeWidth_ = LOWORD(LParam);
            ResizeHeight_ = HIWORD(LParam);
            return 0;
        }
        case WM_SYSCOMMAND: {
            /* Disable ALT application menu. */
            if ((WParam & 0xFFF0) == SC_KEYMENU)
                return 0;
            break;
        }
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
    StartSignal_.acquire();
    JobQueue_.PopAndExecute();

    MSG message{};
    while (GetMessageW(&message, Handle_, 0, 0)) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
}

void
MAIN_WINDOW::JobDispatch()
{
    if (!PostMessageW(Handle_, WM_JOB, 0, 0)) {
        LOG.Error(L"Failed to post job message");
        throw WINDOW_EXCEPTION{"Failed to post job message"};
    }
}

}
