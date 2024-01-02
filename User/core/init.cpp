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
    /* Log session factory */
    GetIoc().RegisterFactory<LOG_SESSION_BASE>([] {

        std::vector<std::shared_ptr<LOG_PROVIDER_BASE>> providers{
            GetIoc().Resolve<DEBUGGER_LOG_PROVIDER_BASE>(),
            GetIoc().Resolve<FILE_LOG_PROVIDER_BASE>()
        };

        return std::make_shared<LOG_SESSION_IMPL>(std::move(providers));
    });

    /* Log providers */
    GetIoc().RegisterFactory<DEBUGGER_LOG_PROVIDER_BASE>([] {
        return std::make_shared<DEBUGGER_LOG_PROVIDER_IMPL>(GetIoc().Resolve<LOG_FORMATTER_BASE>());
    });
    GetIoc().RegisterFactory<FILE_LOG_PROVIDER_BASE>([] {
        return std::make_shared<FILE_LOG_PROVIDER_IMPL>("logs\\log.txt",
                                                   GetIoc().Resolve<LOG_FORMATTER_BASE>());
    });

    /* Log formatter */
    GetIoc().RegisterFactory<LOG_FORMATTER_BASE>([] {
        return std::make_shared<LOG_FORMATTER>();
    });

    /* Log session singleton */
    GetIoc().RegisterSingleton<LOG_SESSION_BASE>([] {
        return GetIoc().Resolve<LOG_SESSION_BASE>();
    });
}

void
InitializeUiSystem()
{
    GetIoc().RegisterFactory<WINDOW_BASE>([](WINDOW_BASE::IOC_PAYLOAD IocParams) {
        return std::make_shared<MAIN_WINDOW>(std::move(IocParams.WindowClass));
    });

    GetIoc().RegisterFactory<WINDOW_CLASS_BASE>([] {
        return std::make_shared<WINDOW_CLASS>();
    });

    GetIoc().RegisterSingleton<WINDOW_CLASS_BASE>([] {
        return GetIoc().Resolve<WINDOW_CLASS_BASE>();
    });
}
