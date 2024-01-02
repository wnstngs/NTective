/*!
 *  @file       logsessn.hpp
 *  @brief      Logging system: Log session.
 */

#pragma once

#include <memory>
#include <vector>

namespace Common::Log {
    class LOG_ENTRY;
    class LOG_PROVIDER_BASE;

    /**
     * @brief Base class for log sessions.
     */
    class LOG_SESSION_BASE {
    public:
        virtual ~LOG_SESSION_BASE() = default;

        /**
         * @brief Writes a log entry to all registered log providers.
         * @param LogEntry The log entry to write.
         */
        virtual
        void
        Write(
            LOG_ENTRY &LogEntry
        ) = 0;

        /**
         * @brief Flushes any buffered log entries in all registered log providers.
         */
        virtual
        void
        Flush() = 0;

        /**
         * @brief Registers a log provider to the session.
         * @param LogProvider The log provider to register.
         */
        virtual
        void
        RegisterProvider(
            std::shared_ptr<LOG_PROVIDER_BASE> LogProvider
        ) = 0;
    };

    /**
     * @brief Log session implementation.
     */
    class LOG_SESSION_IMPL : public LOG_SESSION_BASE {
    public:
        /**
         * @brief Constructs a log session with the specified log providers.
         * @param LogProviders Vector of log providers to initialize the session with.
         */
        LOG_SESSION_IMPL(
            std::vector<std::shared_ptr<LOG_PROVIDER_BASE>> LogProviders = {}
        );

        ~LOG_SESSION_IMPL();

        void
        Write(
            LOG_ENTRY &LogEntry
        ) override;

        void
        Flush() override;

        void
        RegisterProvider(
            std::shared_ptr<LOG_PROVIDER_BASE>
        ) override;

    private:
        std::vector<std::shared_ptr<LOG_PROVIDER_BASE>> LogProviders_;
    };
}
