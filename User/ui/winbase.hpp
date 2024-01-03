/*!
 *  @file       winbase.hpp
 *  @brief      Base window class.
 */

#pragma once

#include <memory>
#include <optional>
#include <string>

#include "winclass.hpp"
#include "../common/win32.h"

namespace Ui {

/*!
 * @brief Abstract base class for windows.
 */
class WINDOW_BASE {
private:
    friend WINDOW_CLASS_BASE;

public:
    virtual ~WINDOW_BASE() = default;

    virtual
    HWND
    GetHandle() = 0;

    /*!
     * @brief IoC container payload class for storing window-related parameters.
     */
    class IOC_PAYLOAD {
    public:
        std::shared_ptr<WINDOW_CLASS_BASE> WindowClass;
        std::optional<std::wstring> Title;
    };

protected:
    virtual
    LRESULT
    HandleMessage(
        HWND Handle,
        UINT Message,
        WPARAM WParam,
        LPARAM LParam
    ) = 0;
};

}
