#pragma once

/**
 * @file mgdl-assert.h
 * @brief Different assert macros
 */

#include <mgdl/mgdl-types.h>

#ifdef __cplusplus
extern "C"
{
#endif
	/**
	 * @brief If the assert fails, break into assert display loop and ask if assert should be ignored.
	*	@details This function is called by the gdl_assert macro. It breaks
	*   normal execution and loops showing the provided error message until user selects
	*   to ignore the assert or quit the program.
	*
	* @param filename    The filename of the code where assert happened. This is provided by __FILE__ macro.
	* @param lineNumber  Line in code where assert happened. This is provided by __LINE__ macro.
	* @param ignoreThis    Pointer to a static bool controlling if the assert will be ignored when it happens again.
	* @param message The message to show when assert fails. Has to contain format specifiers.
	* @param __VA_ARGS__ Arguments for the format specifiers in message.
	*/
	void AssertFunctionPrintf(const char* filename, int lineNumber, bool *ignoreThis, const char* message, ...);

	/**
	 * @brief If the assert fails, break into assert display loop and ask if assert should be ignored.
	*	@details This function is called by the gdl_assert macro. It breaks
	*   normal execution and loops showing the provided error message until user selects
	*   to ignore the assert or quit the program.
	*
	* @param filename    The filename of the code where assert happened. This is provided by __FILE__ macro.
	* @param lineNumber  Line in code where assert happened. This is provided by __LINE__ macro.
	* @param ignoreThis    Pointer to a static bool controlling if the assert will be ignored when it happens again.
	* @param message The message to show when assert fails.
	*/
	void AssertFunctionPrint(const char* filename, int lineNumber, bool *ignoreThis, const char* message);


#ifdef __cplusplus
}
#endif

/* muffintrap:
	This code is mostly from Game Programming Gems 1 article
	"Squeezing More Out of Assert" written by Steve Rabin
*/
#ifdef GEKKO
	// The ##__VA_ARGS is needed in GNU C++17 so that
	// the variadic parameters are optional. Otherwise the comma
	// after message causes compile error.
	// See: https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html
	#define mgdl_assert_printf(test, message, ...) \
	if ((test) == false) \
	{ \
		static bool ignoreThis = false; \
		if (ignoreThis == false) { \
			AssertFunctionPrintf(__FILE__, __LINE__, &ignoreThis, message, ##__VA_ARGS__); \
		} \
	}
#else
	// On Windows and Mac the ##__VA_ARGS__ does not work

#ifdef MGDL_PLATFORM_MAC
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wvariadic-macros"
#endif

	#define mgdl_assert_printf(test, message, ...) \
	if ((test) == false) \
	{ \
		static bool ignoreThis = false; \
		if (ignoreThis == false) { \
			AssertFunctionPrintf(__FILE__, __LINE__, &ignoreThis, message, __VA_ARGS__); \
		} \
	}

#ifdef MGDL_PLATFORM_MAC
	#pragma clang diagnostic pop
#endif

#endif

// These are same for PC and Wii

#define mgdl_assert_print(test, message) \
if ((test) == false) \
{ \
    static bool ignoreThis = false; \
    if (ignoreThis == false) { \
		AssertFunctionPrint(__FILE__, __LINE__, &ignoreThis, message); \
	} \
}

#define mgdl_assert_test(test) \
if ((test) == false) \
{ \
    static bool ignoreThis = false; \
    if (ignoreThis == false) { \
		AssertFunctionPrint(__FILE__, __LINE__, &ignoreThis, #test); \
	} \
}


