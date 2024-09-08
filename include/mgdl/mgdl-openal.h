#pragma once

// includes the OpenAL needed by platform
#ifdef GEKKO
    #include <AL/al.h>
    #include <AL/alc.h>
#else
    #ifdef __APPLE__
        #include <OpenAL/al.h>
        #include <OpenAL/alc.h>
    #else
        #include <AL/al.h>
        #include <AL/alc.h>
    #endif
#endif