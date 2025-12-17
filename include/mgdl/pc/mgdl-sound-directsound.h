#pragma once

#if defined(MGDL_WINDOWS_NATIVE)

#include <dsound.h>

struct SoundDirectSound
{

	LPDIRECTSOUNDBUFFER buffer;
};
typedef struct SoundDirectSound SoundDirectSound;


#endif