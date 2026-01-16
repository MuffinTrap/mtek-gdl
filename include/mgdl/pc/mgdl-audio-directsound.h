#pragma once

#if defined(MGDL_PLATFORM_WINDOWS)

#include <dsound.h>
#include <mgdl/mgdl-types.h>


struct SoundDirectSound
{
	LPDIRECTSOUNDBUFFER buffer;
	DWORD channels;
	sizetype sizeBytes;
	float elapsedSeconds;
	u16 samplerate;
	bool inUse;
};
typedef struct SoundDirectSound SoundDirectSound;


#endif