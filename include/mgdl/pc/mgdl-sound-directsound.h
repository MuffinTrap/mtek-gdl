#pragma once

#if defined(MGDL_WINDOWS_NATIVE)

#include <dsound.h>

struct SoundDirectSound
{
	LPDIRECTSOUNDBUFFER buffer;
	DWORD channels;
};
typedef struct SoundDirectSound SoundDirectSound;


#endif