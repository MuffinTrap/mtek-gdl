#include <mgdl/mgdl-openal.h>
#include <mgdl/mgdl-logger.h>

#include <iostream>

#ifndef GEKKO

void check_al_errors(const char* filename, const sizetype line)
{
    ALCenum error = alGetError();
    if(error != AL_NO_ERROR)
    {
        Log_ErrorF("OpenAL error at %s : %z", filename, line);
        switch(error)
        {
        case AL_INVALID_NAME:
            Log_Error("AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function");
            break;
        case AL_INVALID_ENUM:
            Log_Error("AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function");
            break;
        case AL_INVALID_VALUE:
            Log_Error("AL_INVALID_VALUE: an invalid value was passed to an OpenAL function");
            break;
        case AL_INVALID_OPERATION:
            Log_Error("AL_INVALID_OPERATION: the requested operation is not valid");
            break;
        case AL_OUT_OF_MEMORY:
            Log_Error("AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory");
            break;
        default:
            Log_ErrorF("UNKNOWN AL ERROR: %d", error);
        }
    }
}

#endif
