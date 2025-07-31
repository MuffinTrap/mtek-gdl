#pragma once

/**
 * @file mgdl-vector.h Header for including the vector library.
 *
 * The default Vector library is ccVector but it can be changed
 * by providing macro definitions.
 */

#if defined(MGDL_USE_CCVECTOR)

	#if defined(MGDL_PLATFORM_WII)
		// On the Wii can use CCVector or gu
		// Wii gcc does not complain about ccVector's tricks
		#include <mgdl/ccVector/ccVector.h>
	#else
		// ccVector is written in C and uses anonymous structs
		// to implement vector swizzling. Anonymous structs
		// are not "allowed" in ISO C++ but they work
		#pragma GCC diagnostic push

		#if defined(MGDL_PLATFORM_WINDOWS)
			// Only MSYS UCRT64 GCC complains about -Wpedantic
			#pragma GCC diagnostic ignored "-Wpedantic"
		#else
			// Linux GCC complains about anon types and gnu extension
			#pragma GCC diagnostic ignored "-Wnested-anon-types"
			#pragma GCC diagnostic ignored "-Wgnu-anonymous-struct"
		#endif

		#include <mgdl/ccVector/ccVector.h>

		#pragma GCC diagnostic pop
	#endif
	typedef mat4x4 MTX4x4;
	typedef mat3x3 MTX3x3;
	typedef vec3 V3f;

#elif defined(MGDL_USE_CGLM)

	#include <cglm/cglm.h>

#elif defined(MGDL_PLATFORM_WII)

	#include <ogc/gu.h>

	typedef Mtx44 MTX4x4;
	typedef Mtx33 MTX3x3;
	typedef guVector V3f;

	typedef struct _vecf2 {
		f32 x, y;
	} vec2;

#else

	#error "No vector library specified"

#endif
