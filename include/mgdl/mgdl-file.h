#pragma once
/**
 * @file mgdl-file
 * @brief Cross platform file operations because of windows.
 */

#include "mgdl-types.h"
#include <stdio.h>

namespace gdl
{
    /**
     * @brief Opens a memory buffer as a FILE pointer.
     *
     * @param buffer Memory location.
     * @param size Size of memory buffer in bytes.
     * @param mode File mode: r or w.
     * @return FILE pointer to the memory.
     */
    FILE* BufferOpen(u8* buffer, size_t size, const char* mode);
};
