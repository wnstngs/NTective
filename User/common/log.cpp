/*!
 *  @file       log.cpp
 *  @brief      Logging system.
 */

#include "log.hpp"
#include "win32.h"

DEBUGGER_LOG_PROVIDER::DEBUGGER_LOG_PROVIDER()
{
}

void
DEBUGGER_LOG_PROVIDER::Write(const LOG_ENTRY &LogEntry)
{
    OutputDebugStringW(LogEntry.LogData.c_str());
}

void
DEBUGGER_LOG_PROVIDER::Flush()
{
}

FILE_LOG_PROVIDER::FILE_LOG_PROVIDER(
    const std::filesystem::path &Path
)
{
    create_directories(Path.parent_path());
    File_.open(Path, std::wofstream::out | std::wofstream::app);
}

void
FILE_LOG_PROVIDER::Write(const LOG_ENTRY &LogEntry)
{
    File_ << LogEntry.LogData.c_str();
}

void
FILE_LOG_PROVIDER::Flush()
{
    File_.flush();
}

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
    LOG_SESSION_IMPL *Session
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
