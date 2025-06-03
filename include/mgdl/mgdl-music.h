#pragma once

#include <mgdl/mgdl-sound.h>

#ifndef GEKKO
#include <mgdl/mgdl-openal.h>
#endif

enum MusicType
{
	MusicOgg,
	MusicWav
};

struct Music
{
	// File information
	char* filenameChar;

	MusicType type;

#ifdef GEKKO
	FILE* oggFile;
#else // PC platform
	ALuint source;
#endif
	Sound* wav;

	// Playback variables
	bool isLooping;
	float secondsOffset;
};

extern "C"
{

void Music_Init(Music* music);
Music* Music_LoadOgg(const char* fileName);
Music* Music_LoadWav(const char* fileName);

void Music_DeleteData(Music* music);

/**
 * @brief Plays a music file.
 * @details Plays a music file in the background. Unlike the gdl::Sound class which loads the sound data entirely into
 * memory, this one streams audio data off of a music file which is ideal for music tracks as they tend to be lengthy and take up a bit of memory.
 *
 * @note If the new music cannot be loaded, this function will attempt to play previous music again
 * @note To resume a stopped or paused music, use this function but specify NULL as the file name.
 * @note This port of the library only supports mono or stereo OGG Vorbis audio files. Looping OGG files (logg) and mp3s are
 *	    not supported.
 *
 *	@param[in]	loop		Loop flag (music will be looped if true).
 * @returns True if playback started.
 */

bool Music_Play(Music* music, bool loop);

/*!
 *  \details Pauses the current playing music.
 */
void Music_SetPaused(Music* music, bool paused);

void Music_SetElapsedSeconds(Music* music, float seconds);
float Music_GetElapsedSeconds(Music* music);

//! Stops the currently playing music.
void Music_Stop(Music* music);

bool Music_GetLooping(Music* music);
void Music_SetLooping(Music* music, bool looping);

SoundStatus Music_GetStatus(Music* music);

}
