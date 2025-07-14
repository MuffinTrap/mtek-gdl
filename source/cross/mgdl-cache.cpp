#include <mgdl/mgdl-cache.h>

#ifdef GEKKO
#include <mgdl/wii/mgdl-wii.h>
void mgdl_CacheFlushRange ( void* rangeStart, size_t size )
{
	DCFlushRange(rangeStart, size);
}
#endif

#ifdef MGDL_PLATFORM_LINUX

	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wunused-parameter"
	void mgdl_CacheFlushRange(void* rangeStart, size_t size)
	{
		// nop
	}
#endif
