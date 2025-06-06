#pragma once

/**
 * @file mgdl-vector.h Header for including the vector library.
 *
 * The default Vector library is ccVector but it can be changed
 * by providing macro definitions.
 */

#ifdef MGDL_USE_CCVECTOR

	#ifdef GEKKO

		// Wii gcc does not complain about ccVector's tricks
		#include "ccVector/ccVector.h"

	#else
		// ccVector is written in C and uses anonymous structs
		// to implement vector swizzling. Anonymous structs
		// are not "allowed" in ISO C++ but they work
		#pragma GCC diagnostic push

		#ifdef MGDL_PLATFORM_WINDOWS
			// Only MSYS UCRT64 GCC complains about -Wpedantic
			#pragma GCC diagnostic ignored "-Wpedantic"
		#else
			// Linux GCC complains about anon types and gnu extension
			#pragma GCC diagnostic ignored "-Wnested-anon-types"
			#pragma GCC diagnostic ignored "-Wgnu-anonymous-struct"
		#endif

		#include "ccVector/ccVector.h"

		#pragma GCC diagnostic pop

	#endif
#endif
