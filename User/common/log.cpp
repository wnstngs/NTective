/*!
 *  @file       log.cpp
 *  @brief      Logging system.
 */

#include "log.hpp"
#include "win32.h"

std::wstring
LOG_FORMATTER::FormatLogEntry(
    const LOG_ENTRY &LogEntry
)
{
    std::wostringstream stream;
    std::wstring logLevelName = LogLevelAsString(LogEntry.LogLevel);

    stream << std::format(L"@{} {{{}}} {}",
                          logLevelName,
                          std::chrono::zoned_time{std::chrono::current_zone(), LogEntry.LogTimestamp},
                          LogEntry.LogData);
    stream << std::format(L"\n  >> at {}\n     {}({})\n",
                          LogEntry.FunctionName,
                          LogEntry.SourceFileName,
                          LogEntry.SourceLine);

    return stream.str();
}

DEBUGGER_LOG_PROVIDER::DEBUGGER_LOG_PROVIDER(
    std::shared_ptr<LOG_FORMATTER_BASE> LogFormatter
) : LogFormatter_(std::move(LogFormatter))
{
}

void
DEBUGGER_LOG_PROVIDER::Write(
    const LOG_ENTRY &LogEntry
)
{
    if (LogFormatter_) {
        OutputDebugStringW(LogFormatter_->FormatLogEntry(LogEntry).c_str());
    } else {
        OutputDebugStringW(LogEntry.LogData.c_str());
    }
}

void
DEBUGGER_LOG_PROVIDER::Flush()
{
}

void
DEBUGGER_LOG_PROVIDER::RegisterFormatter(
    std::shared_ptr<LOG_FORMATTER_BASE> LogFormatter
)
{
    LogFormatter_ = std::move(LogFormatter);
}

FILE_LOG_PROVIDER::FILE_LOG_PROVIDER(
    const std::filesystem::path &Path,
    std::shared_ptr<LOG_FORMATTER_BASE> LogFormatter
) : LogFormatter_(std::move(LogFormatter))
{
    create_directories(Path.parent_path());
    File_.open(Path, std::wofstream::out | std::wofstream::app);
}

void
FILE_LOG_PROVIDER::Write(
    const LOG_ENTRY &LogEntry
)
{
    if (LogFormatter_) {
        File_ << LogFormatter_->FormatLogEntry(LogEntry).c_str();
    } else {
        File_ << LogEntry.LogData.c_str();
    }
}

void
FILE_LOG_PROVIDER::Flush()
{
    File_.flush();
}

void
FILE_LOG_PROVIDER::RegisterFormatter(
    std::shared_ptr<LOG_FORMATTER_BASE> LogFormatter
)
{
    LogFormatter_ = std::move(LogFormatter);
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
