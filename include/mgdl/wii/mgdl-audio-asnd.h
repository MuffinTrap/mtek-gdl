/*!	\file mgdl-audio-asnd.h
 *	\brief Wii Audio system.
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
	void	*	buffer;
	short	voiceNumber; // ASND voice number
	int		sSize;
	int		channels;
	bool	inUse;
}; typedef struct SoundASND SoundASND;

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
