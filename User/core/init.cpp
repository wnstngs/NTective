/*!
 *  @file       init.cpp
 *  @brief      Initialization of all subsystems.
 */

#include "init.hpp"
#include "../common/ioccont.hpp"
#include "../common/logprov.hpp"
#include "../common/logsessn.hpp"
#include "../ui/window.hpp"

using namespace Common::Ioc;
using namespace Common::Log;

void
InitializeLoggingSystem()
{
    IOC::Get().RegisterFactory<LOG_SESSION_BASE>([] {

        std::vector<std::shared_ptr<LOG_PROVIDER_BASE>> providers{
            IOC::Get().Resolve<DEBUGGER_LOG_PROVIDER>(),
            IOC::Get().Resolve<FILE_LOG_PROVIDER>()
        };

        return std::make_shared<LOG_SESSION_IMPL>(std::move(providers));
    });

    IOC::Get().RegisterFactory<DEBUGGER_LOG_PROVIDER>([] {
        return std::make_shared<DEBUGGER_LOG_PROVIDER>(IOC::Get().Resolve<LOG_FORMATTER_BASE>());
    });
    IOC::Get().RegisterFactory<FILE_LOG_PROVIDER>([] {
        return std::make_shared<FILE_LOG_PROVIDER>("logs\\log.txt",
                                                   IOC::Get().Resolve<LOG_FORMATTER_BASE>());
    });

    IOC::Get().RegisterFactory<LOG_FORMATTER_BASE>([] {
        return std::make_shared<LOG_FORMATTER>();
    });

    IOC::Get().RegisterSingleton<LOG_SESSION_BASE>([] {
        return IOC::Get().Resolve<LOG_SESSION_BASE>();
    });
}

void
InitializeUiSystem()
{
    IOC::Get().RegisterFactory<WINDOW_BASE>([](WINDOW_BASE::IOC_PAYLOAD IocParams) {
        return std::make_shared<MAIN_WINDOW>(std::move(IocParams.WindowClass));
    });

    IOC::Get().RegisterFactory<WINDOW_CLASS_BASE>([] {
        return std::make_shared<WINDOW_CLASS>();
    });

    IOC::Get().RegisterSingleton<WINDOW_CLASS_BASE>([] {
        return IOC::Get().Resolve<WINDOW_CLASS_BASE>();
    });
}
