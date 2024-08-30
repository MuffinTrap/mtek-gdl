#pragma once
// Cross platform file operations
// because of windows


#include "mgdl-types.h"
#include <stdio.h>
namespace gdl
{
    FILE* BufferOpen(u8* buffer, size_t size, const char* mode);
};
