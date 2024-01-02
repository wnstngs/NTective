/*!
 *  @file       log.hpp
 *  @brief      Logging system: Log entry and controller.
 */

#pragma once

#include <chrono>
#include <string>

namespace Common::Log {

class LOG_SESSION_BASE;

enum class LOG_LEVEL {
    Critical,
    Error,
    Warning,
    Info,
    Verbose
};

/**
 * @brief Converts log level to a string representation.
 * @param LogLevel The log level to convert.
 * @return A string representation of the log level.
 */
inline
std::wstring
LogLevelAsString(
    const LOG_LEVEL LogLevel
)
{
    switch (LogLevel) {

    case LOG_LEVEL::Critical:
        return L"Critical";
    case LOG_LEVEL::Error:
        return L"Error";
    case LOG_LEVEL::Warning:
        return L"Warning";
    case LOG_LEVEL::Info:
        return L"Info";
    case LOG_LEVEL::Verbose:
        return L"Verbose";
    }

    return {};
}

/**
 * @brief Represents an individual log entry.
 */
class LOG_ENTRY {
public:
    std::wstring LogData;
    LOG_LEVEL LogLevel;
    const wchar_t *SourceFileName;
    const wchar_t *FunctionName;
    int SourceLine;
    std::chrono::system_clock::time_point LogTimestamp;
};

/**
 * @brief Controller for managing log entries and session.
 */
class LOG_CONTROLLER : LOG_ENTRY {
public:
    /**
     * @brief Constructs a log controller with source file information.
     * @param SourceFile The source file name.
     * @param Function The function name.
     * @param Line The source line number.
     */
    LOG_CONTROLLER(
        const wchar_t *SourceFile,
        const wchar_t *Function,
        int Line
    );

    /**
     * @brief Records an error-level log message.
     * @param Message The error message to be logged.
     * @return Reference to the current log controller.
     */
    LOG_CONTROLLER &
    Error(
        std::wstring Message
    );

    /**
     * @brief Records a warning-level log message.
     * @param Message The warning message to be logged.
     * @return Reference to the current log controller.
     */
    LOG_CONTROLLER &
    Warning(
        std::wstring Message
    );

    /**
     * @brief Records an info-level log message.
     * @param Message The info message to be logged.
     * @return Reference to the current log controller.
     */
    LOG_CONTROLLER &
    Info(
        std::wstring Message
    );

    /**
     * @brief Records a critical-level log message.
     * @param Message The critical message to be logged.
     * @return Reference to the current log controller.
     */
    LOG_CONTROLLER &
    Critical(
        std::wstring Message
    );

    /**
     * @brief Records a verbose-level log message.
     * @param Message The verbose message to be logged.
     * @return Reference to the current log controller.
     */
    LOG_CONTROLLER &
    Verbose(
        std::wstring Message
    );

    /**
     * @brief Associates a log session with the controller.
     * @param Session The log session to associate.
     * @return Reference to the current log controller.
     */
    LOG_CONTROLLER &
    InSession(
        LOG_SESSION_BASE *Session
    );

    LOG_CONTROLLER &
    At(
        LOG_LEVEL Level
    );

    LOG_CONTROLLER &
    WithMessage(
        std::wstring Message
    );

    ~LOG_CONTROLLER();

private:
    LOG_SESSION_BASE *LogSession_ = nullptr;
};

LOG_SESSION_BASE *
GetDefaultSession();

/*!
 * @brief Basic macro for chain logging.
 * For example: LOG.Error("My error message");
 */
#define LOG Common::Log::LOG_CONTROLLER{ __FILEW__, __FUNCTIONW__, __LINE__ }.InSession(Common::Log::GetDefaultSession())

};
