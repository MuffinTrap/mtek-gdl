#ifdef GEKKO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gccore.h>
#include <asndlib.h>
#include <oggplayer.h>

#include <mgdl/mgdl-music.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-assert.h>

#include "mgdl/wii/mgdl-wii-config.h"
#include "mgdl/wii/mgdl-wii-globals.h"
#include "mgdl/wii/mgdl-wii-globals-internal.h"
#include "mgdl/wii/mgdl-wii-main.h"
#include "mgdl/wii/mgdl-wii-sound.h"
#include "mgdl/wii/mgdl-wii-assert.h"

Music* Music_Create(void)
{
	Music* music = (Music*)malloc(sizeof(Music));
	music->oggFile = NULL;
	music->secondsOffset = 0.0f;
	music->wav = NULL;
	return music;
}


Music* Music_LoadOgg(const char *filename)
{
	Music* music = Music_Create();

	music->filenameChar = (char*)malloc(sizeof(char) * strlen(filename) + 1);
	strcpy(music->filenameChar, filename);

	Log_InfoF("Loading Ogg %s\n", filename);
	music->oggFile = fopen(filename, "r");
	if (music->oggFile != NULL)
	{
		music->type = MusicOgg;
		return music;
	}
	else
	{
		free(music);
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
	if (StatusOgg() == OGG_STATUS_RUNNING)
	{
		return SoundStatus::Playing;
	}
	else
	{
		return SoundStatus::Paused;
	}
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




#endif // GEKKO
