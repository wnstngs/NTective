#pragma once

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "ui/win32.h"

enum class LOG_LEVEL {
    Critical,
    Error,
    Warning,
    Info,
    Verbose
};

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

class LOG_ENTRY {
public:
    std::wstring LogData;
    LOG_LEVEL LogLevel;
    const wchar_t *SourceFileName;
    const wchar_t *FunctionName;
    const wchar_t *SourceLine;
};

class LOG_PROVIDER_BASE {
public:
    virtual ~LOG_PROVIDER_BASE() = default;

    virtual
    void
    Write(
        const LOG_ENTRY &LogEntry
    ) = 0;

    virtual
    void
    Flush() = 0;
};

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

class FILE_LOG_PROVIDER : public LOG_PROVIDER_BASE {
public:
    FILE_LOG_PROVIDER(const std::filesystem::path &Path);

    void
    Write(
        const LOG_ENTRY &LogEntry
    ) override;

    void
    Flush() override;

private:
    std::wofstream File_;
};

class LOG_SESSION_BASE {
public:
    virtual ~LOG_SESSION_BASE() = default;

    virtual
    void
    Write(
        LOG_ENTRY &LogEntry
    ) = 0;

    virtual
    void
    Flush() = 0;

    virtual
    void
    RegisterProvider(
        std::shared_ptr<LOG_PROVIDER_BASE>
    ) = 0;
};

class LOG_SESSION : public LOG_SESSION_BASE {
public:
    LOG_SESSION(std::vector<std::shared_ptr<LOG_PROVIDER_BASE>> LogProviders = {});

    ~LOG_SESSION();

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

class LOG_CONTROLLER : private LOG_ENTRY {
public:
    LOG_CONTROLLER(
        const wchar_t *SourceFile,
        const wchar_t *Function,
        const wchar_t *Line
    );

    LOG_CONTROLLER &
    Error(
        std::wstring Message
    );

    LOG_CONTROLLER &
    Warning(
        std::wstring Message
    );

    LOG_CONTROLLER &
    Info(
        std::wstring Message
    );

    LOG_CONTROLLER &
    Critical(
        std::wstring Message
    );

    LOG_CONTROLLER &
    Verbose(
        std::wstring Message
    );

    LOG_CONTROLLER &
    Session(
        LOG_SESSION *Session
    );

    ~LOG_CONTROLLER();

private:
    LOG_SESSION_BASE *LogSession_ = nullptr;
};
