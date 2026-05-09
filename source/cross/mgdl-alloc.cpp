#include <mgdl/mgdl-alloc.h>
#include <mgdl/mgdl-assert.h>
#include <stdlib.h>

static sizetype allocatedGraphicsMemory = 0;
static sizetype allocatedGeneralMemory = 0;

#ifdef GEKKO
#include <valloc.h>

void* mgdl_AllocateAlignedMemory(sizetype size) {
	return aligned_alloc(32, size);
}

#else // Win-Mac-Linux

void* mgdl_AllocateAlignedMemory(sizetype size) {
	ASSERT_DEBUG(size > 0);
	return malloc(size);
}
#endif

void* mgdl_AllocateGraphicsMemory(sizetype size)
{
	allocatedGraphicsMemory += size;
	return mgdl_AllocateAlignedMemory(size);
}
void mgdl_FreeGraphicsMemory(void* ptr)
{
	ASSERT_DEBUG(ptr != nullptr);
	free(ptr);
}

void* mgdl_AllocateGeneralMemory(sizetype size)
{
	ASSERT_DEBUG(size > 0);
	allocatedGeneralMemory += size;
#ifdef GEKKO
	return valloc(size);
#else
	return malloc(size);
#endif
}
void mgdl_FreeGeneralMemory(void* ptr)
{
	ASSERT_DEBUG(ptr != nullptr);
#ifdef GEKKO
	vfree(ptr);
#else
	free(ptr);
#endif

}
sizetype mgdl_GetAllocatedGraphicsMemoryBytes()
{
	return allocatedGraphicsMemory;
}
sizetype mgdl_GetAllocatedGeneralMemoryBytes()
{
	return allocatedGeneralMemory;
}

