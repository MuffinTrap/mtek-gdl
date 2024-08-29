#pragma once

/** mingw-ucrt on Windows does not have aligned_alloc(alignment, size), so this is 
* a workaround since the alignment is only needed on Wii anyway.
*/

#include <stdlib.h>

namespace gdl
{
	//! Use this function when memory needs to be aligned.
	//  Needs to be aligned for graphical data on Wii.
	void* AllocateAlignedMemory(size_t size);
};
