/*!
 *  @file       init.cpp
 *  @brief      Initialization of all subsystems.
 */

#include "init.hpp"
#include "../common/ioc.hpp"
#include "../common/logprov.hpp"
#include "../common/logsessn.hpp"
#include "../ui/winbase.hpp"
#include "../ui/winimpl.hpp"

using namespace Common;
using namespace Common::Log;

void
InitializeLoggingSystem()
{
    /* Log session factory */
    Ioc::GetIoc().RegisterFactory<LOG_SESSION_BASE>([] {

        std::vector<std::shared_ptr<LOG_PROVIDER_BASE>> providers{
            Ioc::GetIoc().Resolve<DEBUGGER_LOG_PROVIDER_BASE>(),
            Ioc::GetIoc().Resolve<FILE_LOG_PROVIDER_BASE>()
        };

        return std::make_shared<LOG_SESSION_IMPL>(std::move(providers));
    });

    /* Log providers */
    Ioc::GetIoc().RegisterFactory<DEBUGGER_LOG_PROVIDER_BASE>([] {
        return std::make_shared<DEBUGGER_LOG_PROVIDER_IMPL>(Ioc::GetIoc().Resolve<LOG_FORMATTER_BASE>());
    });
    Ioc::GetIoc().RegisterFactory<FILE_LOG_PROVIDER_BASE>([] {
        return std::make_shared<FILE_LOG_PROVIDER_IMPL>("logs\\log.txt",
                                                        Ioc::GetIoc().Resolve<LOG_FORMATTER_BASE>());
    });

    /* Log formatter */
    Ioc::GetIoc().RegisterFactory<LOG_FORMATTER_BASE>([] {
        return std::make_shared<LOG_FORMATTER>();
    });

    /* Log session singleton */
    Ioc::GetSingletons().RegisterDelegateFactory<LOG_SESSION_BASE>();
}

void
InitializeUiSystem()
{
    /* Main window */
    Ioc::GetIoc().RegisterFactory<Ui::WINDOW_BASE>([](Ui::WINDOW_BASE::IOC_PAYLOAD IocParams) {
        return std::make_shared<Ui::MAIN_WINDOW>(IocParams.WindowClass
                                                     ? IocParams.WindowClass
                                                     : Ioc::GetSingletons().Resolve<Ui::WINDOW_CLASS_BASE>(),
                                                 IocParams.Title.value_or(L"NTective — NT Detective"));
    });

    /* Window class factory */
    Ioc::GetIoc().RegisterFactory<Ui::WINDOW_CLASS_BASE>([] {
        return std::make_shared<Ui::WINDOW_CLASS>();
    });

    /* Window class singleton */
    Ioc::GetSingletons().RegisterDelegateFactory<Ui::WINDOW_CLASS_BASE>();
}
