/*!
 *  @file       winimpl.hpp
 *  @brief      Implementation of the base window class.
 */

#pragma once

#include <thread>
#include <semaphore>

#include "backend.hpp"
#include "imguimgr.hpp"
#include "winbase.hpp"
#include "../common/excption.hpp"
#include "../core/jobqueue.hpp"

namespace Ui {

class WINDOW_EXCEPTION : public Common::Util::BUF_EXCEPTION {
public:
    using BUF_EXCEPTION::BUF_EXCEPTION;
};

/*!
* @brief Main application window class. This is the first window that is
* created when the application is launched.
*/
class MAIN_WINDOW : public WINDOW_BASE {

public:
    MAIN_WINDOW(
        std::shared_ptr<WINDOW_CLASS_BASE> WindowClass,
        const std::wstring &Title
    );

    ~MAIN_WINDOW() override;

    HWND
    GetHandle() override;

    GFX_BACKEND &
    GetRenderer() override;

    bool
    IsClosing() override;

protected:
    static constexpr UINT WM_JOB = WM_USER + 0;

    LRESULT
    HandleMessage(
        HWND Handle,
        UINT Message,
        WPARAM WParam,
        LPARAM LParam
    ) override;

    virtual
    void
    MessageLoop();

    template<std::invocable F>
    auto
    Dispatch(F &&Function)
    {
        auto future = JobQueue_.Enqueue(std::forward<F>(Function));
        JobDispatch();
        return future;
    }

    void
    JobDispatch();

protected:
    std::shared_ptr<WINDOW_CLASS_BASE> WindowClass_;
    HWND Handle_;
    std::unique_ptr<GFX_BACKEND> GfxBackend_;
    std::unique_ptr<IMGUI_MGR> ImguiMgr_;
    std::thread MessageLoopThread_;
    std::binary_semaphore StartSignal_{ 0 };
    mutable Core::JOB_QUEUE JobQueue_;
    std::atomic<bool> IsClosing_ = false;
    std::atomic<unsigned int> ResizeWidth_ = 0;
    std::atomic<unsigned int> ResizeHeight_ = 0;

};

}
