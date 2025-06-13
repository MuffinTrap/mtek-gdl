#pragma once
#include <mgdl/mgdl-types.h>

/**
 * @file mgdl-cache.h
 * @brief Function for flushing cache after writing to memory.
 */


#ifdef __cplusplus

extern "C"
{

#endif
	/**
	 * @brief Used to flus the cache after writing to memory.
	 *
	 * This function is needed on some operations on the Wii.
	 *
	 * @param rangeStart Memory address to flush.
	 * @param size Size of the memory in bytes
	 */
	void mgdl_CacheFlushRange(void* rangeStart, sizetype size);

#ifdef __cplusplus
}
#endif
