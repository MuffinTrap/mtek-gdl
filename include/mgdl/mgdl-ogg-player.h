#pragma once

#include <mgdl/mgdl-sound.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct stb_vorbis;
struct MusicOgg
{
	struct stb_vorbis* vorbisfile;
	int lengthSamples;
	float lengthSeconds;
	int sampleRate;
	int channels;
	float elapsedSeconds;
	sizetype sizeBytes;
};
typedef struct MusicOgg MusicOgg;

void OggPlayer_Init();
void OggPlayer_Deinit();
Sound OggPlayer_LoadSound(const char* filename);
void OggPlayer_PlaySound(Sound* snd);
void OggPlayer_StopSound(Sound* snd);
sizetype OggPlayer_GetSoundSizeBytes(Sound* snd);

#ifdef __cplusplus
}
#endif