/*!
 *  @file       logprov.cpp
 *  @brief      Logging system: Log providers and formatting.
 */

#include "logprov.hpp"
#include "log.hpp"
#include "win32.h"

namespace Common::Log {

std::wstring
LOG_FORMATTER::FormatLogEntry(
    const LOG_ENTRY &LogEntry
)
{
    std::wostringstream stream;
    std::wstring logLevelName = LogLevelAsString(LogEntry.LogLevel);

    stream << std::format(L"[{}] [{}] {}",
                          logLevelName,
                          std::chrono::zoned_time{std::chrono::current_zone(), LogEntry.LogTimestamp},
                          LogEntry.LogData);
    stream << std::format(L"\n  >> at {} [{} @ {}]\n",
                          LogEntry.FunctionName,
                          LogEntry.SourceFileName,
                          LogEntry.SourceLine);

    return stream.str();
}

DEBUGGER_LOG_PROVIDER_IMPL::DEBUGGER_LOG_PROVIDER_IMPL(
    std::shared_ptr<LOG_FORMATTER_BASE> LogFormatter
) : LogFormatter_(std::move(LogFormatter))
{
}

void
DEBUGGER_LOG_PROVIDER_IMPL::Write(
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
DEBUGGER_LOG_PROVIDER_IMPL::Flush()
{
}

void
DEBUGGER_LOG_PROVIDER_IMPL::RegisterFormatter(
    std::shared_ptr<LOG_FORMATTER_BASE> LogFormatter
)
{
    LogFormatter_ = std::move(LogFormatter);
}

FILE_LOG_PROVIDER_IMPL::FILE_LOG_PROVIDER_IMPL(
    const std::filesystem::path &Path,
    std::shared_ptr<LOG_FORMATTER_BASE> LogFormatter
) : LogFormatter_(std::move(LogFormatter))
{
    create_directories(Path.parent_path());
    File_.open(Path, std::wofstream::out | std::wofstream::app);
}

void
FILE_LOG_PROVIDER_IMPL::Write(
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
FILE_LOG_PROVIDER_IMPL::Flush()
{
    File_.flush();
}

void
FILE_LOG_PROVIDER_IMPL::RegisterFormatter(
    std::shared_ptr<LOG_FORMATTER_BASE> LogFormatter
)
{
    LogFormatter_ = std::move(LogFormatter);
}

}
