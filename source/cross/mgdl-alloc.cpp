#include <mgdl/mgdl-alloc.h>
#include <stdlib.h>

#ifdef GEKKO
#include <valloc.h>

void* mgdl_AllocateAlignedMemory(sizetype size) {
	return aligned_alloc(32, size);
}

#else // Win-Mac-Linux

void* mgdl_AllocateAlignedMemory(sizetype size) {
	return malloc(size);
}
#endif

void* mgdl_AllocateGraphicsMemory(sizetype size)
{
	return malloc(size);
}
void mgdl_FreeGraphicsMemory(void* ptr)
{
	free(ptr);
}

void* mgdl_AllocateGeneralMemory(sizetype size)
{
#ifdef GEKKO
	return valloc(size);
#else
	return malloc(size);
#endif
}
void mgdl_FreeGeneralMemory(void* ptr)
{
#ifdef GEKKO
	vfree(ptr);
#else
	free(ptr);
#endif

}

