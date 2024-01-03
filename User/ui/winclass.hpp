/*!
 *  @file       winclass.hpp
 *  @brief      Window class management.
 */

#pragma once

#include <string>

#include "../common/win32.h"

namespace Ui {

/*!
 * @brief Abstract base class for window classes.
 */
class WINDOW_CLASS_BASE {
public:
    virtual ~WINDOW_CLASS_BASE() = default;

    virtual
    ATOM
    GetAtom() = 0;

    virtual
    HINSTANCE
    GetInstance() = 0;

protected:
    static
    LRESULT
    ForwardMessage(
        class WINDOW_BASE *Window,
        HWND Handle,
        UINT Message,
        WPARAM WParam,
        LPARAM LParam
    );
};

class WINDOW_CLASS : public WINDOW_CLASS_BASE {
public:
    WINDOW_CLASS(
        const std::wstring &ClassName = DefaultWindowClassName
    );

    ~WINDOW_CLASS() override;

    ATOM
    GetAtom() override;

    HINSTANCE
    GetInstance() override;

protected:
    static constexpr std::wstring DefaultWindowClassName = L"NTective$UM";

private:
    static
    LRESULT
    CALLBACK

    HandleMessageSetup(
        HWND Handle,
        UINT Message,
        WPARAM WParam,
        LPARAM LParam
    );

    static
    LRESULT
    CALLBACK

    HandleMessageThunk(
        HWND Handle,
        UINT Message,
        WPARAM WParam,
        LPARAM LParam
    );

    HINSTANCE Instance_;
    ATOM Atom_;
};

}
