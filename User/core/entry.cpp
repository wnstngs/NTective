/*!
 *  @file       entry.cpp
 *  @brief      Entry point of the NTective program.
 */

#include <iostream>
#include <string>

#include "../common/ioccont.hpp"
#include "../ui/window.hpp"

void
InitializeUserPhase0()
{
    IOC_CONTAINER::Get().RegisterFactory<WINDOW_BASE>([](WINDOW_BASE::IOC_PAYLOAD IocParams) {
        return std::make_shared<MAIN_WINDOW>(std::move(IocParams.WindowClass));
    });

    IOC_CONTAINER::Get().RegisterFactory<WINDOW_CLASS_BASE>([] {
        return std::make_shared<WINDOW_CLASS>();
    });

    IOC_CONTAINER::Get().RegisterSingleton<WINDOW_CLASS_BASE>([] {
        return IOC_CONTAINER::Get().Resolve<WINDOW_CLASS_BASE>();
    });
}

int
WINAPI
wWinMain(
    HINSTANCE Instance,
    HINSTANCE PrevInstance,
    PWSTR CmdLine,
    int CmdShow
)
{
    try {
        InitializeUserPhase0();
    } catch (const std::exception &) {
        // TODO: Log
    } catch (...) {
        // TODO: Log
    }

    return 0;
}
