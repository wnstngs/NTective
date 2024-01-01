/*!
 *  @file       log.hpp
 *  @brief      Logging system.
 */

#pragma once

#include <chrono>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

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
    const wchar_t *SourceLine;
    std::chrono::system_clock::time_point LogTimestamp;
};

/**
 * @brief Base class for log providers.
 */
class LOG_PROVIDER_BASE {
public:
    virtual ~LOG_PROVIDER_BASE() = default;

    /**
     * @brief Writes a log entry.
     * @param LogEntry The log entry to write.
     */
    virtual
    void
    Write(
        const LOG_ENTRY &LogEntry
    ) = 0;

    /**
     * @brief Flushes any buffered log entries.
     */
    virtual
    void
    Flush() = 0;
};

/**
 * @brief Log provider that outputs log entries to the MSVC debugger.
 */
class DEBUGGER_LOG_PROVIDER : public LOG_PROVIDER_BASE {
public:
    DEBUGGER_LOG_PROVIDER();

    void
    Write(
        const LOG_ENTRY &LogEntry
    ) override;

    void
    Flush() override;
};

/**
 * @brief Log provider that writes log entries to a file.
 */
class FILE_LOG_PROVIDER : public LOG_PROVIDER_BASE {
public:
    /**
     * @brief Constructs a FILE_LOG_PROVIDER with the specified file path.
     * @param Path The path to the log file.
     */
    FILE_LOG_PROVIDER(
        const std::filesystem::path &Path
    );

    void
    Write(
        const LOG_ENTRY &LogEntry
    ) override;

    void
    Flush() override;

private:
    std::wofstream File_;
};

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
        const wchar_t *Line
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
    Session(
        LOG_SESSION_IMPL *Session
    );

    ~LOG_CONTROLLER();

private:
    LOG_SESSION_BASE *LogSession_ = nullptr;
};
