#pragma once

#include "mgdl-types.h"
#include <mgdl/mgdl-openal.h>

#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

#ifndef GEKKO
#include <sndfile.h>
#endif

enum SoundStatus
{
	Stopped,
	Playing,
	Paused,
	Initial
};

typedef enum SoundStatus SoundStatus;

//! Sound handling struct
/*!
 *	\details Class object for loading and playing back sound effects with pitch control and stereo panning.
 */
struct Sound
{
	#ifdef GEKKO

	short	format;
	u16		freq;
	void	*sData;
	short	voiceNumber; // ASND voice number

	#else // PC Platform

	SNDFILE* sndfile;
	ALuint buffer, source;

	#endif

	sizetype		sSize; // Size in bytes

	float secondsOffset; // This is mainly to allow chaning the play position on Ogg on Wii for debug purposes
	bool isLooping;
};
typedef struct Sound Sound;

#ifdef __cplusplus
extern "C"
{
#endif


//! Loads a sound file.
/*!
 *	\details This function loads a sound file
 *  which can then be played with Play() or Play2D().
 *
 *	\note This port of the library only supports
 * uncompressed Microsoft WAV sound files with a sample
 * format of 8 or 16-bit and a sample frequency
 * of up to 48KHz, Mono or Stereo.
 *
 *	\param[in]	*fileName	File name of sound file to load.
 *
 *	\returns Pointer if the sound file was successfully loaded, otherwise a null pointer
 */
Sound* Sound_Load(const char* filename);
void Sound_Init(Sound* sound);


//! Deletes the sound data stored in the object
/*!
 *	\details Deallocates the memory buffer containing the loaded sound data.
 */

void Sound_DeleteData(Sound* sound);

//! Plays a sound.
/*!
 *  \details Plays a sound
 */
void Sound_Play(Sound* sound);

//! Plays a sound with extended paramters.
/*!
 *  \details Plays a sound with pitch control and its own volume level.
 *
 *	\param[in]	pitch	Pitch of sound (1.0 is normal pitch).
 *	\param[in]	volume	Volume of sound (100 is full volume).
 */
void Sound_PlayEx(Sound* sound, float pitchOffset, float volumePercent) ;

//! Stops the playback of the sound
/*!
 *	\details This function stops the sound if it is playing
 */

void Sound_Stop(Sound* sound ) ;


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
void Sound_SetElapsedSeconds(Sound* sound, float elapsed) ;


//! Get if the voice is playing or not
/*!
 * \details This function can be used to know if the voice is currently playing
 *
 * \returns True if voice is playing, false if not playing
 */
SoundStatus Sound_GetStatus(Sound* sound ) ;

#ifdef __cplusplus
}
#endif
