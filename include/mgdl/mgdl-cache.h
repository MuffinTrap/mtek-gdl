#pragma once

/**
 * @file mgdl-cache.h
 * @brief Function for flushing cache after writing to memory.
 */

#include <cstdio>

namespace gdl
{
	/**
	 * @brief Used to flus the cache after writing to memory.
	 *
	 * This function is needed on some operations on the Wii
	 *
	 * @param rangeStart Memory address to flush.
	 * @param size Size of the memory in bytes
	 */
	void CacheFlushRange(void* rangeStart, size_t size);
};
