#include <mgdl/mgdl-cache.h>

#ifdef GEKKO
#include <mgdl-wii/mgdl-wii.h>

void gdl::CacheFlushRange ( void* rangeStart, size_t size )
{
	DCFlushRange(rangeStart, size);
}

#else // Win-Mac-Linux

// On these platforms the cache flush does not do anything.

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void gdl::CacheFlushRange ( void* rangeStart, size_t size )
{
	// nop
}
#pragma GCC diagnostic pop
#endif
