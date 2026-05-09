#pragma once

/**
 * @file mgdl-vector.h
 * @ingroup core
 * @brief Header for including the ccvector library.
 * by providing macro definitions.
 */
#if defined(MGDL_PLATFORM_MSYS2)
	// Only MSYS UCRT64 GCC complains about -Wpedantic
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpedantic"
	#elif defined(MGDL_PLATFORM_LINUX) || defined(MGDL_PLATFORM_MAC)
	// Linux GCC complains about anon types and gnu extension
	// ccVector is written in C and uses anonymous structs
	// to implement vector swizzling. Anonymous structs
	// are not "allowed" in ISO C++ but they work
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wnested-anon-types"
	#pragma GCC diagnostic ignored "-Wgnu-anonymous-struct"
#endif

#include <mgdl/ccVector/ccVector.h>

#if defined(MGDL_PLATFORM_LINUX) || defined(MGDL_PLATFORM_MAC) || defined(MGDL_PLATFORM_MSYS2)
	#pragma GCC diagnostic pop
#endif

#define RAYMATH_IMPLEMENTATION
#include <mgdl/raymath/raymath.h>

typedef vec3 V3f;
typedef vec2 V2f;
typedef mat3x3 MTX3x3;
typedef mat4x4 MTX4x4;
