#include <mgdl/mgdl-cache.h>

#ifdef GEKKO
#include <mgdl/wii/mgdl-wii.h>
void mgdl_CacheFlushRange ( void* rangeStart, size_t size )
{
	DCFlushRange(rangeStart, size);
}
#endif

#if defined(MGDL_PLATFORM_LINUX) || defined(MGDL_PLATFORM_MAC) || defined(MGDL_PLATFORM_WINDOWS)

	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wunused-parameter"
	void mgdl_CacheFlushRange(void* rangeStart, size_t size)
	{
		// nop
	}
	#pragma clang diagnostic pop
#endif
