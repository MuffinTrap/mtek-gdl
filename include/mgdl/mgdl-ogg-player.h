#pragma once

#include <mgdl/mgdl-audio.h>
#include <mgdl/mgdl-sound.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct stb_vorbis;
struct MusicOgg
{
	struct stb_vorbis* vorbisfile;
	void* fileBuffer = nullptr;
	int lengthSamples;
	float lengthSeconds;
	int sampleRate;
	int channels;
	float elapsedSeconds;
	sizetype sizeBytes;
	mgdlAudioStateEnum state;
};
typedef struct MusicOgg MusicOgg;

void OggPlayer_Init();
void OggPlayer_Deinit();
Sound OggPlayer_LoadSound(const char* filename);
void OggPlayer_PlaySound(Sound* snd);
void OggPlayer_PauseSound(Sound* snd);
void OggPlayer_ResumeSound(Sound* snd);
void OggPlayer_StopSound(Sound* snd);
sizetype OggPlayer_GetSoundSizeBytes(Sound* snd);
u32 OggPlayer_GetSoundElapsedMs(Sound* snd);
void OggPlayer_SetSoundElapsedMs(Sound* snd, u32 milliseconds);
mgdlAudioStateEnum OggPlayer_GetSoundStatus(Sound* snd);

#ifdef __cplusplus
}
#endif
