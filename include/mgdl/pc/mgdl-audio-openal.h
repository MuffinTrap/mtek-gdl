#pragma once

#include <mgdl/mgdl-openal.h>

#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-sound.h>
#include <mgdl/mgdl-music.h>

#include <cstring>
#include <istream>
#include <fstream>
#include <iostream>

#if defined(MGDL_PLATFORM_LINUX) || defined(MGDL_PLATFORM_MAC) || defined(MGDL_PLATFORM_MSYS2)

struct StreamBuffer
{
	ALuint bufferName;
	void* tempBuffer;
}; typedef struct StreamBuffer StreamBuffer;

struct SoundOpenAL
{
	ALuint buffer, source;
	ALenum format;
	u16 sampleRate;
	ALvoid* tempBuffer;
	sizetype sizeBytes;
	bool inUse;
}; typedef struct SoundOpenAL SoundOpenAL;

#endif 
