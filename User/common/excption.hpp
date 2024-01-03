/*!
 *  @file       excption.hpp
 *  @brief      Custom exception support.
 *
 *  The idea for the assertion and exception infrastructure belongs
 *  to Planetchili of Intel Corp., IntelPresentMon project.
 */

#pragma once

#include <exception>
#include <string>

namespace Common::Util {

class EXCEPTION_BASE : public std::exception {
};

class BUF_EXCEPTION : public EXCEPTION_BASE {
public:
    BUF_EXCEPTION();

    BUF_EXCEPTION(
        std::string Message
    );

    BUF_EXCEPTION(
        const std::wstring &Message
    );

    const char *
    what() const override;

private:
    std::string Message_;
    mutable std::string Buffer_;
};

}
