/*!
 *  @file       stacktrc.hpp
 *  @brief      Stack trace printing.
 */

#pragma once
#include <string>

namespace backward {

class StackTrace;

}

namespace Common::Util {

class STACKTRACE {
public:
    STACKTRACE();

    STACKTRACE(
        const STACKTRACE &Stacktrace
    );

    STACKTRACE &
    operator=(
        const STACKTRACE &Stacktrace
    );

    ~STACKTRACE();

    std::wstring
    Get();
};

}
