/*!
 *  @file       strutil.cpp
 *  @brief      String utility functions.
 */

#include "strutil.hpp"

namespace Common::Util {

std::wstring
StringToWstring(
    const std::string &String
)
{
	std::wstring Wstring;
    Wstring.resize(String.size() + 1);
    size_t actual;
    mbstowcs_s(&actual,
               Wstring.data(),
               Wstring.size(),
               String.c_str(),
               _TRUNCATE);
    if (actual > 0) {
        Wstring.resize(actual - 1);
        return Wstring;
    }
    return {};
}

std::string
WstringToString(
    const std::wstring &Wstring
)
{
    std::string String;
    String.resize(Wstring.size() * 2);
    size_t actual;
    wcstombs_s(&actual,
               String.data(),
               String.size(),
               Wstring.c_str(),
               _TRUNCATE);
    String.resize(actual - 1);
    return String;
}

}
