/*!	\file mgdl-sound.h
 *	\brief Sound module header
 */
#ifndef _GDL_WII_SOUND_H
#define _GDL_WII_SOUND_H

#if defined(GEKKO)


#include <gccore.h>
#include <cstdio>
#include <string>

#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-sound.h>
#include <mgdl/wii/mgdl-wii-config.h>


struct SoundASND
{
	short	format;
	u16		freq;
	void	*sData;
	short	voiceNumber; // ASND voice number
}; typedef struct SoundASND;

struct MusicASND
{
	FILE* oggFile;
};

namespace gdl {
	//! Plays a sound with 2D stereo panning.
	/*!
	 *	\param[in]	pitch	Pitch of sound (1.0 is normal pitch).
	 *	\param[in]	volume	Volume of sound (100 is full volume).
	 *	\param[in]	x		X position of sound (relative to screen).
	 *	\param[in]	y		Y position of sound (this parameter does nothing for now).
	 */
	void Play2D(float pitch, float volume, float x, float y);
};


//! Sets master sound volumes.
/*!
 *  \details Changes the master volumes of music playback and sound effects.
 *
 *	\note Changing the music volume will affect the currently playing music. Already playing sound effects however
 *		will not be affected but the next sounds will be.
 *
 *	\param[in]	musicVol	Master music volume.
 *	\param[in]	soundVol	Sound effects volume.
 */
void SetMasterVolumes(float musicVol, float soundVol);


//! Sets the music volume.
/*!
 *	\note Changing the music volume will affect the currently playing music on-the-fly.
 *
 *	\param[in]	volume	Music volume.
 */
void SetMusicVolume(float volume);

#endif // GEKKO
#endif // _GDL_WII_SOUND_H
