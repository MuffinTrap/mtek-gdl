#ifdef GEKKO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gccore.h>
#include <asndlib.h>
#include <oggplayer.h>

#include <mgdl/mgdl-music.h>

#include "mgdl/wii/mgdl-wii-config.h"
#include "mgdl/wii/mgdl-wii-globals.h"
#include "mgdl/wii/mgdl-wii-globals-internal.h"
#include "mgdl/wii/mgdl-wii-main.h"
#include "mgdl/wii/mgdl-wii-sound.h"
#include "mgdl/wii/mgdl-wii-assert.h"

void Music_Init(Music* music)
{
	music->oggFile = NULL;
	music->secondsOffset = 0.0f;
}


Music* Music_Load(const char *filename)
{
	Music* music = new Music();

	music->filenameChar = new char[strlen(filename)];
	strcpy(music->filenameChar, filename);

	Log_InfoF("Loading Ogg %s\n", filename);
	music->oggFile = fopen(filename, "r");
	if (music->oggFile != NULL)
	{
		return music;
	}
	else
	{
		delete music;
		return nullptr;
	}
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void Music_Play(Music* music, float pitchOffset, float volumePercent)
{
	int playMode = OGG_ONE_TIME;
	if (music->isLooping) {
		playMode = OGG_INFINITE_TIME;
	}

	int po = PlayOggFilePtr(music->oggFile, 0, playMode);
	if (po != 0)
	{
		Log_ErrorF("Failed to play ogg file %s\n", music->filenameChar);
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

Sound_Status Music_GetStatus(Music* music)
{
	if (StatusOgg() == OGG_STATUS_RUNNING)
	{
		return Sound_Status::Playing;
	}
	else
	{
		return Sound_Status::Paused;
	}
}

#pragma GCC diagnostic pop

void Music_SetLooping(Music* music, bool looping)
{
	music->isLooping = looping;
}


#endif // GEKKO
