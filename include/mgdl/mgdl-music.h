#pragma once

#ifndef GEKKO
#include <mgdl/mgdl-openal.h>
#endif

struct Music
{
	// File information
	char* filenameChar;

#ifdef GEKKO
	// Ogg variables
	FILE* oggFile;
#else // PC platform

	ALuint source;

#endif

	// Playback variables
	bool isLooping;
	float secondsOffset;
};

void Music_Init(Music* music);
Music* Music_Load(const char* fileName);

void Music_DeleteData(Music* music);

//!	Plays a music file.
/*!
 *	\details Plays a music file in the background. Unlike the gdl::Sound class which loads the sound data entirely into
 *	    memory, this one streams audio data off of a music file which is ideal for music tracks as they tend to be lengthy
 *      and take up a bit of memory.
 *
 *	\note If the new music cannot be loaded, this function will attempt to play previous music again
 *	\note To resume a stopped or paused music, use this function but specify NULL as the file name.
 *	\note This port of the library only supports mono or stereo OGG Vorbis audio files. Looping OGG files (logg) and mp3s are
 *	    not supported.
 *
 *	\param[in]	fileName	Name of music file to play.
 *	\param[in]	loop		Loop flag (music will be looped if true).
 */
bool Music_Play(Music* music, const char* fileName, bool loop);

//! Pauses the currently playing music.
/*!
 *  \details Pauses the current playing music. To resume playback, use gdl::PlayMusic() but specify NULL as the file name.
 */
void Music_Pause(Music* music);

//! Stops the currently playing music.
void Music_Stop(Music* music);

bool Music_GetLooping(Music* music);

/*! @} */
