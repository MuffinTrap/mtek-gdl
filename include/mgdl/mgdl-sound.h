#pragma once

#include "mgdl-types.h"

#ifdef __cplusplus
#	include <cstdlib>
#else
#	include <stdlib.h>
#endif

/**
 * @file mgdl-sound.h
 * @ingroup audio
 * @brief Sound struct and functions for loading wav files
 */

	enum SoundFileType
	{
		SoundOgg,
		SoundWav,
		SoundMp3
	};
	typedef enum SoundFileType SoundFileType;

	enum SoundSampleFormat
	{
		Format_Mono_8,
		Format_Mono_16,
		Format_Stereo_8,
		Format_Stereo_16
	};
	typedef enum SoundSampleFormat SoundSampleFormat;

	int Sound_FormatToChannels(SoundSampleFormat);
//! Sound handling struct
/*!
 *	\details Class object for loading and playing back sound effects with pitch control and stereo panning.
 */
struct Sound
{
	s32 voiceNumber; // index to array in each platform, -1 if invalid
	float normalizedVolume; /**< Volume between 0 and 1 */
	SoundFileType type;
};
typedef struct Sound Sound;

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Allocates a new Sound with empty variables
 * @return New Sound
 */
Sound* Sound_Create(void);

void Sound_InitEmpty(Sound* sound);
void Sound_Init(Sound* snd, s32 voiceNumber, SoundFileType filetype);

void Sound_ToString(Sound* sound);

/**
 * @brief Structure to have multiple sounds collected together
 */
struct SoundBank
{
	u8 soundAmount;
	u8 currentIndex;
	mgdlSoundBankPlayMode playmode;
	Sound** sounds;
};
typedef struct SoundBank SoundBank;

SoundBank* SoundBank_Create(u8 soundAmount);
SoundBank* SoundBank_Destroy(SoundBank* bank);
void SoundBank_SetSound(SoundBank* bank, u8 index, Sound* sound);
void SoundBank_SetMode(SoundBank* bank, mgdlSoundBankPlayMode mode);
void SoundBank_PlayNext(SoundBank* bank);
void SoundBank_Stop(SoundBank* bank);
bool SoundBank_IsPlaying(SoundBank* bank);


#ifdef __cplusplus
}
#endif
