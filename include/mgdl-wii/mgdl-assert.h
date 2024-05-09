#pragma once

/* muffintrap: 
    This code is mostly from Game Programming Gems 1 article 
    "Squeezing More Out of Assert" written by Steve Rabin
*/

#include <stdio.h>
#include <stdarg.h>

//! If the assert fails, break into assert display loop and ask if assert should be ignored. Only used if _MGDL_DEBUG is defined.
/*!
*	\details This function is called by the gdl_assert macro. It breaks 
*   normal execution and loops showing the provided error message until user selects
*   to ignore the assert or quit the program.
*
*	\param[in]	filename    The filename of the code where assert happened. This is provided by __FILE__ macro.
*	\param[in]	lineNumber  Line in code where assert happened. This is provided by __LINE__ macro.
*	\param[inout]	ignoreToggle    Reference to a static bool controlling if the assert will be ignored when it happens again.
*	\param[in]  message The message to show when assert fails. Can contain format specifiers.
*	\param[in]	__VA_ARGS__ Arguments for the format specifiers in message. This parameter(s) is optional.
*
*/
void AssertFunction(const char* filename, int lineNumber, bool& ignoreToggle, const char* message, ...);

// muffintrap: 
// The ##__VA_ARGS is needed in GNU C++17 so that
// the variadic parameters are optional. Otherwise the comma
// after message causes compile error.
// See: https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html
#define gdl_assert(test, message, ...) \
if ((test) == false) \
{ \
    static bool ignoreThis = false; \
    if (ignoreThis == false) { \
        AssertFunction(__FILE__, __LINE__, ignoreThis, message, ##__VA_ARGS__); \
    } \
}
