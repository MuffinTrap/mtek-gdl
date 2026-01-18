#include <mgdl/mgdl-openal.h>
#include <mgdl/mgdl-logger.h>

#include <iostream>

#if defined(MGDL_PLATFORM_LINUX) || defined(MGDL_PLATFORM_MAC) || defined(MGDL_PLATFORM_MSYS2)

// NOTE This does not play nice with addressSanitizer
void check_al_errors(const char* filename, int line)
{
    ALCenum error = alGetError();
    if(error != AL_NO_ERROR)
    {
        Log_ErrorF("OpenAL error at %s : %d ", filename, line);
        switch(error)
        {
        case AL_INVALID_NAME:
            Log_Error("AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function\n");
            break;
        case AL_INVALID_ENUM:
            Log_Error("AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function\n");
            break;
        case AL_INVALID_VALUE:
            Log_Error("AL_INVALID_VALUE: an invalid value was passed to an OpenAL function\n");
            break;
        case AL_INVALID_OPERATION:
            Log_Error("AL_INVALID_OPERATION: the requested operation is not valid\n");
            break;
        case AL_OUT_OF_MEMORY:
            Log_Error("AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory\n");
            break;
        default:
            Log_ErrorF("UNKNOWN AL ERROR: %d\n", error);
        }
    }
}

#endif
