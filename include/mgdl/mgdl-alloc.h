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
 * @brief Use this function when graphics memory needs to be aligned.
 *
 * @details Memory needs to be aligned for graphical (and audio data?) on Wii. Anything that is read by gpu or audio device
 * is safest to allocate aligned and also flush
 *
 * @param size Size of memory in bytes
 * @return Address of the reserved memory or nullptr on allocation failure.
 */
void* mgdl_AllocateAlignedMemory(sizetype size);

/**
 * @brief Use this function when memory is used by graphics device or the allocation is temporary.
 *
 * @details Memory needs to be aligned for graphical data on Wii. Anything that is read by gpu is safest to allocate aligned and also flush.
 * This memory is faster to allocate and mapped to malloc, so it is faster and the memory is mapped by C runtime.
 *
 * @param size Size of memory in bytes
 * @return Address of the reserved memory or nullptr on allocation failure.
 */
void* mgdl_AllocateGraphicsMemory(sizetype size);

/**
 * @brief Use this function to free allocated graphics memory
 * @param ptr Pointer to the memory
 */
void mgdl_FreeGraphicsMemory(void* ptr);
/**
 * @brief Use this function when memory is not used by graphics device
 *
 * @details On Wii there is 1/3rd reserved for graphics and rest for general use, such as gameplay objects and audio etc. This memory is slower and mapped manually. Use this function when the memory allocation is permanent and does not change during lifetime.
 *
 * @param size Size of memory in bytes
 * @return Address of the reserved memory or nullptr on allocation failure.
 */
void* mgdl_AllocateGeneralMemory(sizetype size);
/**
 * @brief Use this function to free allocated general memory
 * @param ptr Pointer to the memory
 */
void mgdl_FreeGeneralMemory(void* ptr);

#ifdef __cplusplus
}
#endif
