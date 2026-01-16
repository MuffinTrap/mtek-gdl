#pragma once

/**
 * @file mgdl-vector.h
 * @ingroup core
 * @brief Header for including the ccvector library.
 * @details The default Vector library is ccVector but it can be changed
 * by providing macro definitions.
 */

#if defined(MGDL_USE_CCVECTOR)

#	if defined(MGDL_PLATFORM_WII)
		// On the Wii can use CCVector or gu
		// Wii gcc does not complain about ccVector's tricks
		#include <mgdl/ccVector/ccVector.h>
#	elif defined(MGDLPLATFORM_WINDOWS)
		// No problem with Visual C++
#	else
		// ccVector is written in C and uses anonymous structs
		// to implement vector swizzling. Anonymous structs
		// are not "allowed" in ISO C++ but they work
		#pragma GCC diagnostic push

#		if defined(MGDL_PLATFORM_MSYS2)
			// Only MSYS UCRT64 GCC complains about -Wpedantic
			#pragma GCC diagnostic ignored "-Wpedantic"
#		elif defined(MGDL_PLATFORM_LINUX) || defined(MGDL_PLATFORM_MAC)
			// Linux GCC complains about anon types and gnu extension
			#pragma GCC diagnostic ignored "-Wnested-anon-types"
			#pragma GCC diagnostic ignored "-Wgnu-anonymous-struct"
#		endif

	#include <mgdl/ccVector/ccVector.h>

	#pragma GCC diagnostic pop
#	endif

	typedef mat4x4 MTX4x4;
	typedef mat3x3 MTX3x3;
	typedef vec3 V3f;
	typedef vec2 V2f;

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
