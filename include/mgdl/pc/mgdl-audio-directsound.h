#pragma once

#if defined(MGDL_WINDOWS_NATIVE)

#include <dsound.h>


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