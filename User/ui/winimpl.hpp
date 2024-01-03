/*!
 *  @file       winimpl.hpp
 *  @brief      Implementation of the base window class.
 */

#pragma once

#include "winbase.hpp"

namespace Ui {

/*!
* @brief Main application window class. This is the first window that is
* created when the application is launched.
*/
class MAIN_WINDOW : public WINDOW_BASE {

public:
    MAIN_WINDOW(
        std::shared_ptr<WINDOW_CLASS_BASE> WindowClass,
        const std::wstring &Title = DefaultWindowTitle
    );

    ~MAIN_WINDOW() override;

    HWND
    GetHandle() override;

protected:
    static constexpr std::wstring DefaultWindowTitle = L"NTective — NT Detective";

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

    std::shared_ptr<WINDOW_CLASS_BASE> WindowClass_;
    HWND Handle_;
};

}
