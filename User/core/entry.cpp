/*!
 *  @file       entry.cpp
 *  @brief      Entry point of the NTective program.
 */

#include <exception>

#include "init.hpp"
#include "../common/log.hpp"
#include "../common/ioc.hpp"
#include "../common/win32.h"
#include "../ui/winbase.hpp"
#include "../ui/winimpl.hpp"

int
WINAPI
wWinMain(
    HINSTANCE Instance,
    HINSTANCE PrevInstance,
    PWSTR CmdLine,
    int CmdShow
)
{
    UNREFERENCED_PARAMETER(Instance);
    UNREFERENCED_PARAMETER(PrevInstance);
    UNREFERENCED_PARAMETER(CmdLine);
    UNREFERENCED_PARAMETER(CmdShow);

    try {

        /* Register IoC factories and singletons */
        InitializeLoggingSystem();
        InitializeUiSystem();

        std::shared_ptr<Ui::WINDOW_BASE> mainWindow = Common::Ioc::GetIoc().Resolve<Ui::WINDOW_BASE>();

        /* Core loop */
        while (!mainWindow->IsClosing()) {
            std::array color = {1.0f, 0.0f, 0.0f, 1.0f};
            mainWindow->GetRenderer().NewFrame();
            mainWindow->GetRenderer().ClearBuffer(color);
            mainWindow->GetRenderer().EndFrame();
        }

    } catch (const std::exception &e) {
        LOG.Error(Common::Util::StringToWstring(e.what()));
        return -1;
    }

    return 0;
}
