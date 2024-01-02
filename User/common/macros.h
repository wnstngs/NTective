/*!
 *  @file       macros.h
 *  @brief      Common utility macros.
 */

#pragma once

#define Common_Util_CONCAT_(x, y)   x ## y
#define Common_Util_CONCAT(x, y)    CONCAT_(x, y)
#define Common_Util_STRING_(x)      #x
#define Common_Util_STRING(x)       STRING_(x)
#define Common_Util_WSTRING(x)      CONCAT(L, STRING(x))

#define Common_Util_NUM_ARGS_(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, TOTAL, ...) TOTAL
#define Common_Util_NUM_ARGS(...) Common_Util_NUM_ARGS_(__VA_ARGS__,12_,11_,10_,9_,8_,7_,6_,5_,4_,3_,2_,1_)
#define Common_Util_DISPATCH_VA(macro, ...) Common_Util_CONCAT(macro,Common_Util_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)
