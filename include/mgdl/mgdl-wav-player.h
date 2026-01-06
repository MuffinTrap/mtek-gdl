#pragma once

#include <mgdl/mgdl-sound.h>


#ifdef __cplusplus
extern "C" {
#endif

	void WavPlayer_Init();
	void WavPlayer_Deinit();
	Sound WavPlayer_LoadSound(const char* filename);
	void WavPlayer_PlaySound(Sound* snd);
	sizetype WavPlayer_GetSoundSizeBytes(Sound* snd);
	
#ifdef __cplusplus
}
#endif