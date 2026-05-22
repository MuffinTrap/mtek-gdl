#pragma once

/**
 * @file mgdl-vector.h
 * @ingroup core
 * @brief Header for including the ccvector library.
 * by providing macro definitions.
 */

#define RAYMATH_IMPLEMENTATION
#ifdef GEKKO
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#else
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-field-initializers"
#endif

#include <mgdl/raymath/raymath.h>

#ifdef GEKKO
#pragma GCC diagnostic pop
#else
#pragma clang diagnostic pop
#endif
