/*!
 *  @file       log.cpp
 *  @brief      Logging system.
 */

#include "log.hpp"

#include "ioccont.hpp"
#include "logsessn.hpp"

using namespace Common::Ioc;

namespace Common::Log {

LOG_CONTROLLER::LOG_CONTROLLER(
    const wchar_t *SourceFile,
    const wchar_t *Function,
    int Line
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
LOG_CONTROLLER::InSession(
    LOG_SESSION_BASE *Session
)
{
    LogSession_ = Session;
    return *this;
}

LOG_CONTROLLER &
LOG_CONTROLLER::At(
    LOG_LEVEL Level
)
{
    LogLevel = Level;
    return *this;
}

LOG_CONTROLLER &
LOG_CONTROLLER::WithMessage(
    std::wstring Message
)
{
    LogData = std::move(Message);
    return *this;
}

LOG_CONTROLLER::~LOG_CONTROLLER()
{
    if (LogSession_) {
        LogSession_->Write(*this);
    }
}

LOG_SESSION_BASE *
GetDefaultSession()
{
    static auto defaultSession = GetIoc().Resolve<LOG_SESSION_BASE>();
    return defaultSession.get();
}

}
