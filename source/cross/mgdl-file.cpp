#include <mgdl/mgdl-file.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
FILE* BufferOpen(u8* buffer, size_t size, const char* mode)
{
    #ifdef MGDL_PLATFORM_WINDOWS
        FILE* fp = tmpfile();
        if (fp == nullptr)
        {
            perror("Could not open temp file for buffer reading");
            return nullptr;
        }
        for (size_t byte = 0; byte < size; byte++)
        {
            fputc(buffer[byte], fp);
        }
        rewind(fp);
        return fp;

    #else
        return fmemopen((void*)buffer, size, mode);

    #endif
}
#pragma GCC diagnostic pop
