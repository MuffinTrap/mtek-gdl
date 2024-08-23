#include <mgdl/mgdl-alloc.h>
#include <stdlib.h>

#ifdef GEKKO

void* gdl::AllocateAlignedMemory(size_t size) {
	return aligned_alloc(32, size);
}

#else // Win-Mac-Linux

void* gdl::AllocateAlignedMemory(size_t size) {
	return malloc(size);
}

#endif
