/*!
 *  @file       assert.hpp
 *  @brief      Assertion.
 */

#pragma once

#include <sstream>
#include <string>

#include "excption.hpp"
#include "macros.h"

namespace Common::Util {

enum class ASSERTION_EFFECT {
    Log,
    Exception,
    Termination
};

class ASSERTION_FAILED_EXCEPTION : public BUF_EXCEPTION {
    using Base = BUF_EXCEPTION;

public:
    using BUF_EXCEPTION::BUF_EXCEPTION;
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

    [[noreturn]]
    void
    Throw();

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

#define NTECTIVE_ASSERTION(Expression)  (!NTECTIVE_ASSERTIONS_ACTIVE || bool(Expression))    \
                                            ? void(0)                                        \
                                            : (void)Common::Util::ASSERTION{                 \
                                                  Common_Util_WSTRING(Expression),           \
                                                  __FILEW__,                                 \
                                                  __FUNCTIONW__,                             \
                                                  __LINE__                                   \
                                               }

#define NTECTIVE_CHECK(Expression)      bool(Expression)                                            \
                                           ? void(0)                                                \
                                           : (void)Common::Util::ASSERTION{                         \
                                                 Common_Util_WSTRING(Expression),                   \
                                                 __FILEW__,                                         \
                                                 __FUNCTIONW__,                                     \
                                                 __LINE__,                                          \
                                                 NTECTIVE_ASSERTIONS_ACTIVE                         \
                                                     ? Common::Util::ASSERTION_EFFECT::Termination  \
                                                     : Common::Util::ASSERTION_EFFECT::Log          \
                                             }

#define NTECTIVE_CHECK_FAIL(Expression) (void)Common::Util::ASSERTION{                          \
                                            L"[Always Fail]",                                   \
                                            __FILEW__,                                          \
                                            __FUNCTIONW__,                                      \
                                            __LINE__,                                           \
                                            NTECTIVE_ASSERTIONS_ACTIVE                          \
                                                ? Common::Util::ASSERTION_EFFECT::Termination   \
                                                : Common::Util::ASSERTION_EFFECT::Log           \
                                        }

#define WATCH(...)                                      Common_Util_DISPATCH_VA(Common_Util_AW_, __VA_ARGS__)
#define Common_Util_AW_(Expression)                     Watch((Expression), Common_Util_WSTRING(Expression)) 
#define Common_Util_AW_1_(z)                            Common_Util_AW_(z) 
#define Common_Util_AW_2_(z, a)                         Common_Util_AW_(z).Common_Util_AW_(a) 
#define Common_Util_AW_3_(z, a, b)                      Common_Util_AW_(z).Common_Util_AW_(a).Common_Util_AW_(b) 
#define Common_Util_AW_4_(z, a, b, c)                   Common_Util_AW_(z).Common_Util_AW_(a).Common_Util_AW_(b).Common_Util_AW_(c) 
#define Common_Util_AW_5_(z, a, b, c, d)                Common_Util_AW_(z).Common_Util_AW_(a).Common_Util_AW_(b).Common_Util_AW_(c).Common_Util_AW_(d) 
#define Common_Util_AW_6_(z, a, b, c, d, e)             Common_Util_AW_(z).Common_Util_AW_(a).Common_Util_AW_(b).Common_Util_AW_(c).Common_Util_AW_(d).Common_Util_AW_(e) 
#define Common_Util_AW_7_(z, a, b, c, d, e, f)          Common_Util_AW_(z).Common_Util_AW_(a).Common_Util_AW_(b).Common_Util_AW_(c).Common_Util_AW_(d).Common_Util_AW_(e).Common_Util_AW_(f) 
#define Common_Util_AW_8_(z, a, b, c, d, e, f, g)       Common_Util_AW_(z).Common_Util_AW_(a).Common_Util_AW_(b).Common_Util_AW_(c).Common_Util_AW_(d).Common_Util_AW_(e).Common_Util_AW_(f).Common_Util_AW_(g) 
#define Common_Util_AW_9_(z, a, b, c, d, e, f, g, h)    Common_Util_AW_(z).Common_Util_AW_(a).Common_Util_AW_(b).Common_Util_AW_(c).Common_Util_AW_(d).Common_Util_AW_(e).Common_Util_AW_(f).Common_Util_AW_(g).Common_Util_AW_(h)
