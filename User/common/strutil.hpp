/*!
 *  @file       strutil.hpp
 *  @brief      String utility functions.
 */

#pragma once

#include <string>

namespace Common::Util {

std::wstring
StringToWstring(
    const std::string &String
);

std::string
WstringToString(
    const std::wstring &Wstring
);

}
