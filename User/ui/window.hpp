/*!
 *  @file       window.hpp
 *  @brief      Core window management.
 */

#pragma once

#include <memory>
#include <optional>
#include <string>

#include "../common/win32.h"

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

/*!
 * @brief Abstract base class for windows.
 */
class WINDOW_BASE {
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

private:
    friend class WINDOW_CLASS_BASE;
};

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
