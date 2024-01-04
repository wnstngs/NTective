/*!
 *  @file       strutil.cpp
 *  @brief      String utility functions.
 */

#include "strutil.hpp"

#include "log.hpp"
#include "win32.h"

namespace Common::Util {

std::wstring
StringToWstring(
    const std::string &String
)
{
    if (String.empty()) {
        return {};
    }

    const int length = MultiByteToWideChar(CP_ACP,
                                           0,
                                           String.data(),
                                           static_cast<int>(String.size()),
                                           nullptr,
                                           0);
    if (length == 0) {
        LOG.Warning(L"MultiByteToWideChar failed");
        return {};
    }

    std::wstring wstring(static_cast<std::wstring::size_type>(length), 
                         L'\0');

    if (MultiByteToWideChar(CP_ACP,
                            0,
                            String.data(),
                            static_cast<int>(String.size()),
                            wstring.data(),
                            static_cast<int>(wstring.size())) == 0) {
        LOG.Warning(L"MultiByteToWideChar failed");
        return {};
    }

    return wstring;
}

std::string
WstringToString(
    const std::wstring &Wstring
)
{
    if (Wstring.empty()) {
        return {};
    }

    size_t actual;
    std::string string;
    string.resize(Wstring.size() * 2);

    auto status = wcstombs_s(&actual,
                             string.data(),
                             string.size(),
                             Wstring.c_str(),
                             _TRUNCATE);
    if (status > 0) {
        string.resize(actual - 1);

        return string;

    } else {
        LOG.Warning(L"wcstombs_s failed");
        return {};
    }
}

}
