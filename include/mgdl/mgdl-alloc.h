#pragma once

/**
 * @file mgdl-alloc.h
 * @ingroup core
 * @brief Workaround of windows missing aligned_alloc(aligment, size).
 *
 * @details mingw-ucrt on Windows does not have aligned_alloc(alignment, size), so this is
 * a workaround since the alignment is only needed on Wii anyway.
 *
 */

#include <stdlib.h>
#include <mgdl/mgdl-types.h>


#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Use this function when memory needs to be aligned.
 *
 * @details Memory needs to be aligned for graphical and audio data on Wii. Anything that is read by gpu or audio device
 * is safest to allocate aligned and also flush
 *
 * @param size Size of memory in bytes
 * @return Address of the reserved memory or nullptr on allocation failure.
 */
void* mgdl_AllocateAlignedMemory(sizetype size);

#ifdef __cplusplus
}
#endif
