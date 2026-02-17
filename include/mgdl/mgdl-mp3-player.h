#pragma once

#include <mgdl/mgdl-audio.h>
#include <mgdl/mgdl-sound.h>
#include <mgdl/dr_mp3/dr_mp3.h>

#if defined(__cplusplus)
extern "C" {
#endif

struct MusicMp3
{
	drmp3* mp3;
	void* fileBuffer = nullptr;
	int framesElapsed;
	float secondsElapsed;
	sizetype sizeBytes;
	mgdlAudioStateEnum state;
}; typedef struct MusicMp3 MusicMp3;

void Mp3Player_Init();
void Mp3Player_Deinit();
Sound Mp3Player_LoadSound(const char* filename);
void Mp3Player_PlaySound(Sound* snd);
void Mp3Player_PauseSound(Sound* snd);
void Mp3Player_ResumeSound(Sound* snd);
void Mp3Player_StopSound(Sound* snd);


sizetype Mp3Player_GetSoundSizeBytes(Sound* snd);
u32 Mp3Player_GetSoundElapsedMs(Sound* snd);
void Mp3Player_SetSoundElapsedMs(Sound* snd, u32 milliseconds);
mgdlAudioStateEnum Mp3Player_GetSoundStatus(Sound* snd);

#if defined(__cplusplus)
}
#endif
