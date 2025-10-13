#pragma once

#include <mgdl/mgdl-sound.h>

#ifndef GEKKO
#include <mgdl/mgdl-openal.h>
#endif

/**
 * @file mgdl-music.h
 * @ingroup audio asset
 * @brief Music class for different types of music
 */

enum MusicType
{
	MusicOgg,
	MusicWav
};
typedef enum MusicType MusicType;

struct Music
{
	// File information
	char* filenameChar;
	// Playback variables
	bool isLooping;
	float secondsOffset;

	MusicType type;
	Sound* wav;

	#ifdef GEKKO
		FILE* oggFile;
	#else // PC platform
		ALuint source;
	#endif
};
typedef struct Music Music;

#ifdef __cplusplus
extern "C"
{
#endif

Music* Music_Create(void);
Music* Music_LoadOgg(const char* fileName);
Music* Music_LoadWav(const char* fileName);

void Music_DeleteData(Music* music);

/**
 * @brief Plays a music file.
 * @details Plays a music file in the background. Unlike the gdl::Sound class which loads the sound data entirely into
 * memory, this one streams audio data off of a music file which is ideal for music tracks as they tend to be lengthy and take up a bit of memory.
 *
 *	@param[in]	music		The music to start
 *	@param[in]	loop		Loop flag. If true music will loop and you have to call Music_UpdatePlay to make sure it does
 * @returns True if playback started.
 */

bool Music_Play(Music* music, bool loop);

/**
 * @brief Continues playback of a previously started music
 * @details Call this every frame to make sure music keeps playing and looping. If you only want to play music once, this does not need to be called
 *
 *	@param[in]	music		The music to keep playing
 */
void Music_UpdatePlay(Music* music);

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

#ifdef __cplusplus
}
#endif
