#pragma once


extern "C"
{
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
	void AssertFunctionPrintf(const char* filename, int lineNumber, bool &ignoreThis, const char* message, ...);
	void AssertFunctionPrint(const char* filename, int lineNumber, bool &ignoreThis, const char* message);
}

/* muffintrap:
	This code is mostly from Game Programming Gems 1 article
	"Squeezing More Out of Assert" written by Steve Rabin
*/
#ifdef GEKKO
	// The ##__VA_ARGS is needed in GNU C++17 so that
	// the variadic parameters are optional. Otherwise the comma
	// after message causes compile error.
	// See: https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html
	#define gdl_assert_printf(test, message, ...) \
	if ((test) == false) \
	{ \
		static bool ignoreThis = false; \
		if (ignoreThis == false) { \
			AssertFunctionPrintf(__FILE__, __LINE__, ignoreThis, message, ##__VA_ARGS__); \
		} \
	}
#else
	// On Windows and Mac the ##__VA_ARGS__ does not work

	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wvariadic-macros"

	#define gdl_assert_printf(test, message, ...) \
	if ((test) == false) \
	{ \
		static bool ignoreThis = false; \
		if (ignoreThis == false) { \
			AssertFunctionPrintf(__FILE__, __LINE__, ignoreThis, message, __VA_ARGS__); \
		} \
	}

	#pragma clang diagnostic pop

#endif

// These are same for PC and Wii

#define gdl_assert_print(test, message) \
if ((test) == false) \
{ \
    static bool ignoreThis = false; \
    if (ignoreThis == false) { \
		AssertFunctionPrint(__FILE__, __LINE__, ignoreThis, message); \
	} \
}

#define gdl_assert_test(test) \
if ((test) == false) \
{ \
    static bool ignoreThis = false; \
    if (ignoreThis == false) { \
		AssertFunctionPrint(__FILE__, __LINE__, ignoreThis, #test); \
	} \
}


