#pragma once

/**
 * @file mgdl-alloc.h
 * @brief Workaround of windows missing aligned_alloc(aligment, size).
 *
 * @details mingw-ucrt on Windows does not have aligned_alloc(alignment, size), so this is
 * a workaround since the alignment is only needed on Wii anyway.
 */

#include <stdlib.h>
#include <mgdl/mgdl-types.h>

extern "C"
{

/**
 * @brief Use this function when memory needs to be aligned.
 *
 * @details Memory needs to be aligned for graphical data on Wii.
 *
 * @param size Size of memory in bytes
 * @return Address of the reserved memory or nullptr on allocation failure.
 */
void* mgdl_AllocateAlignedMemory(sizetype size);

}
