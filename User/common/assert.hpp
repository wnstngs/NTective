/*!
 *  @file       assert.hpp
 *  @brief      Assertion.
 */

#pragma once

#include <sstream>
#include <string>

#include "macros.h"

namespace Common::Util {

enum class ASSERTION_EFFECT {
    Log,
    Exception,
    Termination
};

class ASSERTION {
public:
    ASSERTION(
        std::wstring Expression,
        const wchar_t *FileName,
        const wchar_t *FunctionName,
        int SourceLine,
        ASSERTION_EFFECT AssertionEffect = ASSERTION_EFFECT::Termination
    );

    ~ASSERTION();

    ASSERTION &
    Message(
        const std::wstring &Message
    );

    template<typename T>
    ASSERTION &
    Watch(
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

private:
    const wchar_t *FileName_;
    const wchar_t *FunctionName_;
    int SourceLine_;
    ASSERTION_EFFECT AssertionEffect_;
    std::wostringstream Stream_;
};

}

#ifndef NTECTIVE_ASSERTIONS_ACTIVE
#ifdef NDEBUG
#define NTECTIVE_ASSERTIONS_ACTIVE false  
#else
#define NTECTIVE_ASSERTIONS_ACTIVE true
#endif
#endif

#define NTECTIVE_ASSERTION(Expression) (!NTECTIVE_ASSERTIONS_ACTIVE || bool(Expression))    \
                                           ? void(0)                                        \
                                           : (void)Common::Util::ASSERTION{                 \
                                                 Common_Util_WSTRING(Expression),           \
                                                __FILEW__,                                  \
                                                 __FUNCTIONW__,                             \
                                                 __LINE__                                   \
                                             }
