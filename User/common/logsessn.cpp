/*!
 *  @file       logsessn.cpp
 *  @brief      Logging system: Log session.
 */

#include "logsessn.hpp"
#include "logprov.hpp"

namespace Common::Log {
    LOG_SESSION_IMPL::LOG_SESSION_IMPL(
        std::vector<std::shared_ptr<LOG_PROVIDER_BASE>> LogProviders
    ) : LogProviders_(std::move(LogProviders))
    {
    }

    LOG_SESSION_IMPL::~LOG_SESSION_IMPL() = default;

    void
    LOG_SESSION_IMPL::Write(
        LOG_ENTRY &LogEntry
    )
    {
        for (const auto &provider : LogProviders_) {
            provider->Write(LogEntry);
        }
    }

    void
    LOG_SESSION_IMPL::Flush()
    {
        for (const auto &provider : LogProviders_) {
            provider->Flush();
        }
    }

    void
    LOG_SESSION_IMPL::RegisterProvider(
        std::shared_ptr<LOG_PROVIDER_BASE> LogProvider
    )
    {
        LogProviders_.push_back(std::move(LogProvider));
    }
}
