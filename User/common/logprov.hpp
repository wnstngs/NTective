/*!
 *  @file       logprov.hpp
 *  @brief      Logging system: Log providers and formatting.
 */

#pragma once

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

namespace Common::Log {

class LOG_ENTRY;

class LOG_FORMATTER_BASE {
public:
    virtual
    ~LOG_FORMATTER_BASE() = default;

    virtual
    std::wstring
    FormatLogEntry(
        const LOG_ENTRY &LogEntry
    ) = 0;
};

class LOG_FORMATTER : public LOG_FORMATTER_BASE {
public:
    std::wstring
    FormatLogEntry(
        const LOG_ENTRY &LogEntry
    ) override;
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

    /**
     * @brief Registers a log entry formatter to the provider.
     * @param LogFormatter The log entry formatter to register.
     */
    virtual
    void
    RegisterFormatter(
        std::shared_ptr<LOG_FORMATTER_BASE> LogFormatter
    ) = 0;
};

/**
 * @brief Log provider that outputs log entries to the MSVC debugger.
 */
class DEBUGGER_LOG_PROVIDER_BASE : public LOG_PROVIDER_BASE {
};

/**
 * @brief Log provider that outputs log entries to the MSVC debugger.
 */
class DEBUGGER_LOG_PROVIDER_IMPL : public DEBUGGER_LOG_PROVIDER_BASE {
public:
    DEBUGGER_LOG_PROVIDER_IMPL(
        std::shared_ptr<LOG_FORMATTER_BASE> LogFormatter = {}
    );

    void
    Write(
        const LOG_ENTRY &LogEntry
    ) override;

    void
    Flush() override;

    void
    RegisterFormatter(
        std::shared_ptr<LOG_FORMATTER_BASE> LogFormatter
    ) override;

private:
    std::shared_ptr<LOG_FORMATTER_BASE> LogFormatter_;
};

/**
 * @brief Log provider that writes log entries to a file.
 */
class FILE_LOG_PROVIDER_BASE : public LOG_PROVIDER_BASE {
};

/**
 * @brief Log provider that writes log entries to a file.
 */
class FILE_LOG_PROVIDER_IMPL : public FILE_LOG_PROVIDER_BASE {
public:
    /**
     * @brief Constructs a FILE_LOG_PROVIDER with the specified file path.
     * @param Path The path to the log file.
     */
    FILE_LOG_PROVIDER_IMPL(
        const std::filesystem::path &Path,
        std::shared_ptr<LOG_FORMATTER_BASE> LogFormatter = {}
    );

    void
    Write(
        const LOG_ENTRY &LogEntry
    ) override;

    void
    Flush() override;

    void
    RegisterFormatter(
        std::shared_ptr<LOG_FORMATTER_BASE> LogFormatter
    ) override;

private:
    std::wofstream File_;
    std::shared_ptr<LOG_FORMATTER_BASE> LogFormatter_;
};

}
