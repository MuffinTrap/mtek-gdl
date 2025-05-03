#include <mgdl/mgdl-alloc.h>
#include <stdlib.h>

#ifdef GEKKO

void* mgdl_AllocateAlignedMemory(sizetype size) {
	return aligned_alloc(32, size);
}

#else // Win-Mac-Linux

void* mgdl_AllocateAlignedMemory(sizetype size) {
	return malloc(size);
}

#endif
