#pragma once

#include <mgdl/mgdl-sound.h>


#ifdef __cplusplus
extern "C" {
#endif

	void WavPlayer_Init();
	void WavPlayer_Deinit();

	Sound WavPlayer_LoadSound(const char* filename);
	void WavPlayer_PlaySound(Sound* snd);
	void WavPlayer_StopSound(Sound* snd);

	sizetype WavPlayer_GetSoundSizeBytes(Sound* snd);
	u32 WavPlayer_GetSoundElapsedMs(Sound* snd);
	void WavPlayer_SetSoundElapsedMs(Sound* snd, u32 milliseconds);
	
#ifdef __cplusplus
}
#endif