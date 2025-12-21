#pragma once

#include "mgdl-types.h"

#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

/**
 * @file mgdl-sound.h
 * @ingroup audio
 * @brief Sound struct and functions for loading wav files
 */

	enum SoundFileType
	{
		SoundOgg,
		SoundWav
	};
	typedef enum SoundFileType SoundFileType;
//! Sound handling struct
/*!
 *	\details Class object for loading and playing back sound effects with pitch control and stereo panning.
 */
struct Sound
{
	s32 voiceNumber; // index to array in each platform, -1 if invalid
	s32 channels;
	sizetype sizeBytes; // Size in bytes
	SoundFileType type;


	float elapsedSeconds; // How long the sound has been playing. Used when streaming
	bool isLooping;
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
void Sound_Init(Sound* snd, s32 voiceNumber, s32 channels, sizetype sizeBytes, SoundFileType filetype);

Sound* Sound_Load(const char* filename);

//! Deletes the sound data stored in the object
/*!
 *	\details Deallocates the memory buffer containing the loaded sound data.
 */

void Sound_DeleteData(Sound* sound);

//! Plays a sound with extended paramters.
/*!
 *  \details Plays a sound with pitch control and its own volume level.
 *
 *	\param[in]	pitch	Pitch of sound (1.0 is normal pitch).
 *	\param[in]	volume	Volume of sound (100 is full volume).
 */
void Sound_PlayEx(Sound* sound, float pitchOffset, float volumePercent) ;

//! Pauses the playback of the sound
/*!
 *	\details This function pauses the sound if it is playing
 */
void Sound_SetPaused(Sound* sound, bool pause) ;
void Sound_SetLooping(Sound* sound, bool looping) ;
bool Sound_GetLooping(Sound* sound );

//! Get elapsed playback time in seconds
/*!
 *	\details This function returns how long the sound has been playing in seconds
 *
 *	\returns Elapsed playback time in seconds
 */
float Sound_GetElapsedSeconds(Sound* sound ) ;

#ifdef __cplusplus
}
#endif
