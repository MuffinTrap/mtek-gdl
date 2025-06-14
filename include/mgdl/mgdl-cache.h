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
#ifdef GEKKO
	void mgdl_CacheFlushRange(void* rangeStart, sizetype size);
#else
	// If the function is defined in the .cpp file
	// the clang compiler leaves it out and then the 
	// linker complains that it cannot find the definition...
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
	void mgdl_CacheFlushRange(void* rangeStart, size_t size)
	{
		// nop
	}
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
}
#endif
