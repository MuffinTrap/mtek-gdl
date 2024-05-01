#pragma once

/* This code is mostly from Game Programming Gems 1 article 
Squeezing More Out of Assert by Steve Rabin
*/

#ifndef _MGDL_DEBUG

namespace gdl {
    //! If the assert fails, break into assert display loop and ask if assert should be ignored. Only used if _MGDL_DEBUG is defined.
    /*!
    *	\details This function is called by the mgdl_assert macro. It breaks 
    *   normal execution and loops showing the provided error message until user selects
    *   to ignore the assert or quit the program.
    *
    *	\param[in]	message The message to show when assert fails.
    *	\param[in]	lineNumber Line in code where assert happened. This is provided by __LINE__ macro.
    *	\param[in]	filename The filename of the code where assert happened. This is provided by __FILE__ macro.
    *	\param[inout]	ignoreToggle Reference to a static bool controlling if the assert will be ignored when it happens again.
    *
    */
    void AssertFunction(const char* message, int lineNumber, const char* filename, bool& ignoreToggle);

    #define _assert(test, message) \
    if (test == false) \
    { \
        static bool ignoreThis = false; \
        if (ignoreThis == false) { \
            gdl::AssertFunction(message, __LINE__, __FILE__, ignoreThis); \
        } \
    }
#else
    #define assert(test, message)

#endif
};
