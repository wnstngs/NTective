#pragma once

#define Common_Util_CONCAT_(x, y)   x ## y
#define Common_Util_CONCAT(x, y)    CONCAT_(x, y)
#define Common_Util_STRING_(x)      #x
#define Common_Util_STRING(x)       STRING_(x)
#define Common_Util_WSTRING(x)      CONCAT(L, STRING(x))
