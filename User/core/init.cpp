/*!
 *  @file       init.cpp
 *  @brief      Initialization of all subsystems.
 */

#include "init.hpp"
#include "../common/ioccont.hpp"
#include "../common/log.hpp"
#include "../ui/window.hpp"

void
InitializeLoggingSystem()
{
    IOC_CONTAINER::Get().RegisterFactory<LOG_SESSION_BASE>([] {

        std::vector<std::shared_ptr<LOG_PROVIDER_BASE>> providers{
            IOC_CONTAINER::Get().Resolve<DEBUGGER_LOG_PROVIDER>(),
            IOC_CONTAINER::Get().Resolve<FILE_LOG_PROVIDER>()
        };

        return std::make_shared<LOG_SESSION_IMPL>(std::move(providers));
    });

    IOC_CONTAINER::Get().RegisterFactory<DEBUGGER_LOG_PROVIDER>([] {
        return std::make_shared<DEBUGGER_LOG_PROVIDER>();
    });
    IOC_CONTAINER::Get().RegisterFactory<FILE_LOG_PROVIDER>([] {
        return std::make_shared<FILE_LOG_PROVIDER>("logs\\log.txt");
    });

    IOC_CONTAINER::Get().RegisterSingleton<LOG_SESSION_BASE>([] {
        return IOC_CONTAINER::Get().Resolve<LOG_SESSION_BASE>();
    });
}

void
InitializeUiSystem()
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
