#ifdef GEKKO

#if 0 // Wii uses stb_vorbis too

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gccore.h>
#include <asndlib.h>
#include <oggplayer.h>

#include <mgdl/mgdl-music.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-assert.h>
#include "mgdl/mgdl-assert.h"

#include "mgdl/wii/mgdl-wii-config.h"
#include "mgdl/wii/mgdl-wii-globals.h"
#include "mgdl/wii/mgdl-wii-globals-internal.h"
#include "mgdl/wii/mgdl-wii-main.h"
#include "mgdl/wii/mgdl-wii-sound.h"

Music* Music_Create(void)
{
	Music* music = (Music*)malloc(sizeof(Music));
	music->oggFile = NULL;
	music->secondsOffset = 0.0f;
	music->wav = NULL;
	music->isLooping = false;
	music->filenameChar = nullptr;
	music->type = MusicOgg;
	return music;
}


Music* Music_LoadOgg(const char *filename)
{
	Log_InfoF("Loading Ogg %s\n", filename);
	FILE* oggFile = fopen(filename, "r");
	if (oggFile != NULL)
	{
		Music* music = Music_Create();
		music->filenameChar = (char*)malloc(sizeof(char) * strlen(filename) + 1);
		strcpy(music->filenameChar, filename);
		music->oggFile = oggFile;
		music->type = MusicOgg;

		return music;
	}
	else
	{
		return nullptr;
	}
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
Music* Music_LoadWav(const char* filename)
{
	mgdl_assert_print(false, "Loading wav music on Wii takes up lots of memory! Use Ogg instead!");
    Log_InfoF("Loading music from %s\n", filename);

    Sound* snd = Sound_Load(filename);
    if (snd)
    {
        Music* music = Music_Create();
        music->type = MusicWav;
        music->wav = snd;
        return music;
    }
	return nullptr;
}

bool Music_Play(Music* music, bool loop)
{
	music->isLooping = loop;
	if (music->type == MusicOgg)
	{
		int playMode = OGG_ONE_TIME;
		if (loop) {
			playMode = OGG_INFINITE_TIME;
		}

		int po = PlayOggFilePtr(music->oggFile, 0, playMode);
		if (po != 0)
		{
			Log_ErrorF("Failed to play ogg file %s\n", music->filenameChar);
			return false;
		}
		return true;
	}
	else if (music->type == MusicWav)
	{
		Sound_Play(music->wav);
		return true;
	}
	else
	{
		return false;
	}
}

void Music_UpdatePlay(Music* music)
{
	SoundStatus status = Music_GetStatus(music);
	// Restart looping music if it has stopped or failed to loop
    if (music->isLooping == true && (status == SoundStopped || status == SoundLoopFailed))
    {
		Music_Stop(music);
        Music_Play(music, music->isLooping);
    }
}

void Music_DeleteData(Music* music)
{
}

float Music_GetElapsedSeconds(Music* music)
{
	return music->secondsOffset + (float)GetTimeOgg()/1000.0f;
}

void Music_SetElapsedSeconds(Music* music, float seconds)
{
	SetTimeOgg(s32(seconds*1000.0f));
	music->secondsOffset = seconds;
}

void Music_SetPaused(Music* music, bool pause)
{
	if (pause)
	{
		PauseOgg(1);
	}
	else
	{
		PauseOgg(0);
	}
}

void Music_Stop(Music* music)
{
	StopOgg();
}


SoundStatus Music_GetStatus(Music* music)
{
	int status = StatusOgg();
	switch(status)
	{
		case OGG_STATUS_RUNNING:
			return SoundStatus::SoundPlaying;
		break;
		case OGG_STATUS_PAUSED:
			return SoundStatus::SoundPaused;
		break;
		case OGG_STATUS_ERR:
			return SoundStatus::SoundError;
		break;
		case OGG_STATUS_EOF:
			return SoundStatus::SoundStopped;
		break;
		case OGG_STATUS_LOOP_FAIL:
			return SoundStatus::SoundLoopFailed;
		break;
	}
	return SoundStatus::SoundInitial;
}
#pragma GCC diagnostic pop


void Music_SetLooping(Music* music, bool looping)
{
	music->isLooping = looping;
}

bool Music_GetLooping(Music* music)
{
	return music->isLooping;
}



#endif // 0

#endif // GEKKO
