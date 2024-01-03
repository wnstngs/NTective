/*!
 *  @file       assert.cpp
 *  @brief      Assertion utility.
 */

#include "assert.hpp"
#include "log.hpp"
#include "logsessn.hpp"

using namespace Common::Log;

namespace Common::Util {

ASSERTION::ASSERTION(
    std::wstring Expression,
    const wchar_t *FileName,
    const wchar_t *FunctionName,
    int SourceLine,
    ASSERTION_EFFECT AssertionEffect
) : FileName_(FileName),
    FunctionName_(FunctionName),
    SourceLine_(SourceLine),
    AssertionEffect_(AssertionEffect)
{
    Stream_
        << L"Assertion Failed! "
        << Expression
        << "\n";
}

ASSERTION::~ASSERTION()
{
    LOG.InSession(Log::GetDefaultSession())
       .WithMessage(Stream_.str())
       .At(AssertionEffect_ == ASSERTION_EFFECT::Termination
               ? LOG_LEVEL::Critical
               : LOG_LEVEL::Error);

    if (AssertionEffect_ == ASSERTION_EFFECT::Termination) {
        Log::GetDefaultSession()->Flush();
        std::terminate();
    }
}

ASSERTION &
ASSERTION::Message(const std::wstring &Message)
{
    Stream_
        << L"  Message: "
        << Message
        << L"\n";
    return *this;
}

template<typename T>
ASSERTION &
ASSERTION::Watch(
    T &&Value,
    const wchar_t *Name
)
{
    Stream_
        << L"\t"
        << Name
        << L" => "
        << std::forward<T>(Value)
        << L"\n";
    return *this;
}

void
ASSERTION::Throw()
{
    AssertionEffect_ = ASSERTION_EFFECT::Exception;
    throw ASSERTION_FAILED_EXCEPTION{};
}

}
