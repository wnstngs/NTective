/*!
 *  @file       log.cpp
 *  @brief      Logging system.
 */

#include "log.hpp"
#include "logsessn.hpp"

namespace Common::Log {
    LOG_CONTROLLER::LOG_CONTROLLER(
        const wchar_t *SourceFile,
        const wchar_t *Function,
        const wchar_t *Line
    ) : LOG_ENTRY{
        .SourceFileName = SourceFile,
        .FunctionName = Function,
        .SourceLine = Line,
        .LogTimestamp = std::chrono::system_clock::now()
    }
    {
    }

    LOG_CONTROLLER &
    LOG_CONTROLLER::Error(
        std::wstring Message
    )
    {
        LogData = std::move(Message);
        LogLevel = LOG_LEVEL::Error;
        return *this;
    }

    LOG_CONTROLLER &
    LOG_CONTROLLER::Warning(
        std::wstring Message
    )
    {
        LogData = std::move(Message);
        LogLevel = LOG_LEVEL::Warning;
        return *this;
    }

    LOG_CONTROLLER &
    LOG_CONTROLLER::Info(
        std::wstring Message
    )
    {
        LogData = std::move(Message);
        LogLevel = LOG_LEVEL::Info;
        return *this;
    }

    LOG_CONTROLLER &
    LOG_CONTROLLER::Critical(
        std::wstring Message
    )
    {
        LogData = std::move(Message);
        LogLevel = LOG_LEVEL::Critical;
        return *this;
    }

    LOG_CONTROLLER &
    LOG_CONTROLLER::Verbose(
        std::wstring Message
    )
    {
        LogData = std::move(Message);
        LogLevel = LOG_LEVEL::Verbose;
        return *this;
    }

    LOG_CONTROLLER &
    LOG_CONTROLLER::Session(
        LOG_SESSION_BASE *Session
    )
    {
        LogSession_ = Session;
        return *this;
    }

    LOG_CONTROLLER::~LOG_CONTROLLER()
    {
        if (LogSession_) {
            LogSession_->Write(*this);
        }
    }
}
