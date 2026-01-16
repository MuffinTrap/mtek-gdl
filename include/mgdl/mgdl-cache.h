#pragma once
#include <mgdl/mgdl-types.h>

/**
 * @file mgdl-cache.h
 * @ingroup core
 * @brief Function for flushing cache after writing to memory.
 */


#if defined(__cplusplus)
extern "C"
{
#endif
	/**
	 * @brief Used to flus the cache after writing to memory.
	 *
	 * This function is needed after code has modified memory that is read by gpu or audio device.
	 *
	 * @param rangeStart Memory address to flush.
	 * @param size Size of the memory in bytes
	 */
#ifdef GEKKO
	void mgdl_CacheFlushRange(void* rangeStart, sizetype size);
#endif

#if defined(MGDL_PLATFORM_WINDOWS)
	void mgdl_CacheFlushRange(void* rangeStart, sizetype size);
#endif

#if defined(MGDL_PLATFORM_MSYS2)
	// If the function is defined in the .cpp file
	// the clang compiler leaves it out and then the
	// linker complains that it cannot find the definition...
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wunused-parameter"
	void mgdl_CacheFlushRange(void* rangeStart, sizetype size)
	{
		// nop
	}
	#pragma clang diagnostic pop
#endif

#if defined(MGDL_PLATFORM_LINUX) || defined(MGDL_PLATFORM_MAC)
	void mgdl_CacheFlushRange(void* rangeStart, sizetype size);
#endif

#ifdef __cplusplus
}
#endif
