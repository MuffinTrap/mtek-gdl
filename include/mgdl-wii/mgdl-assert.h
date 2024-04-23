#pragma once

/* This code is mostly from Game Programming Gems 1 article 
Squeezing More Out of Assert by Steve Rabin
*/

#ifdef _MGDL_DEBUG

namespace gdl {
    //! If the assert fails, break into assert display loop and ask if assert should be ignored. Only used if _MGDL_DEBUG is defined.
    /*!
    *	\details This function is called by the mgdl_assert macro. It breaks 
    *   normal execution and loops showing the provided error message until user selects
    *   to ignore the assert or quit the program.
    *
    *	\param[in]	test    The boolean result from assert. 
    *	\param[in]	message The message to show when assert fails.
    *	\param[in]	lineNumber Line in code where assert happened. This is provided by __LINE__ macro.
    *	\param[in]	filename The filename of the code where assert happened. This is provided by __FILE__ macro.
    *	\param[inout]	ignoreToggle Reference to a static bool controlling if the assert will be ignored when it happens again.
    *
    */
    void AssertFunction(bool test, char* message, int lineNumber, char* filename, bool& ignoreToggle);
};

    #define mgdl_assert(test, message) \
    { \
        static bool ignoreThis = false; \
        if (!ignoreThis) { \
            AssertFunction(test, message, __LINE__, __FILE__, &ignoreThis); \
        } \
    }
#else
    #define mgdl_assert(test, message)

#endif
