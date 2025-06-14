#include <mgdl/mgdl-cache.h>

#ifdef GEKKO
#include <mgdl/wii/mgdl-wii.h>
void mgdl_CacheFlushRange ( void* rangeStart, size_t size )
{
	DCFlushRange(rangeStart, size);
}
#endif
