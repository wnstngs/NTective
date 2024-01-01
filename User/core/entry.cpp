/*!
 *  @file       entry.cpp
 *  @brief      Entry point of the NTective program.
 */

#include <exception>

#include "init.hpp"
#include "../common/win32.h"

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
        InitializeLoggingSystem();
        InitializeUiSystem();
    } catch (const std::exception &) {
        // TODO: Log
    } catch (...) {
        // TODO: Log
    }

    return 0;
}
