// Sound class module functions

#ifndef _GDL_WII_SOUND
#define _GDL_WII_SOUND

#if GDL_NO_SOUND == FALSE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gccore.h>
#include <asndlib.h>
#include <oggplayer.h>

#include "mgdl-wii/mgdl-wii-config.h"
#include "mgdl-wii/mgdl-wii-globals.h"
#include "mgdl-wii/mgdl-wii-globals-internal.h"
#include "mgdl-wii/mgdl-wii-main.h"
#include "mgdl-wii/mgdl-wii-sound.h"
#include "mgdl-wii/mgdl-wii-assert.h"


gdl::SoundWii::SoundWii() {

	format=0;
	freq=0;
	sData=NULL;
	sSize=0;
	voiceNumber = SND_INVALID;
}

gdl::SoundWii::~SoundWii() {

	if (sData != NULL)
		vfree(sData);

}

bool gdl::SoundWii::LoadFile(const char* fileName) {

	// Open up the wave file
	if (gdl::ConsoleActive)
		printf("gdl: Opening sound file %s... ", fileName);


	FILE *fp;

	if (!(fp = fopen(fileName, "r"))) {
		gdl::CallErrorCallback("Sound file %s not found", fileName);
		return(false);
	}
	return LoadSound(fp);
}

bool gdl::SoundWii::LoadBuffer(const u8* buffer, size_t size)
{
	FILE *fp;

	uint8_t* wavBuffer = (uint8_t*)malloc(size);
	memcpy(wavBuffer, buffer, size);
	DCFlushRange(wavBuffer, size);

	bool openOk = true;
	if (!(fp = fmemopen(wavBuffer, size, "r")))
	{
		openOk = false;
		gdl::CallErrorCallback("Sound buffer could not be opened");
	}
	if (openOk)
	{
		openOk = LoadSound(fp);
	}
	free(wavBuffer);
	return openOk;
}

bool gdl::SoundWii::LoadSound(FILE* fp)
{
	// Get header chunk
	struct {
		char	id[4];
		int		size;
		char	format[4];
	} WAV_Chunk;

	fread(&WAV_Chunk, 1, sizeof(WAV_Chunk), fp);

	if (memcmp(&WAV_Chunk.id, "RIFF", 4) || memcmp(&WAV_Chunk.format, "WAVE", 4)) {
		gdl::CallErrorCallback("Sound file is not a Microsoft WAVE file");
		fclose(fp);
		return(false);
	}

	gdl::wii::RevBytes(&WAV_Chunk.size, sizeof(int));


	// Get header chunk
	struct {
		char	id[4];
		int		size;
		short	format;
		short	chan;
		int		freq;
		int		brate;
		short	balign;
		short	bps;
	} WAV_Subchunk1;

	fread(&WAV_Subchunk1, 1, sizeof(WAV_Subchunk1), fp);


	// Convert values we need into BE
	gdl::wii::RevBytes(&WAV_Subchunk1.size, sizeof(int));
	gdl::wii::RevBytes(&WAV_Subchunk1.format, sizeof(short));
	gdl::wii::RevBytes(&WAV_Subchunk1.chan, sizeof(short));
	gdl::wii::RevBytes(&WAV_Subchunk1.freq, sizeof(int));
	gdl::wii::RevBytes(&WAV_Subchunk1.bps, sizeof(short));

	if (memcmp(&WAV_Subchunk1.id, "fmt ", 4)) {
		gdl::CallErrorCallback("Unsupported WAV format variant in sound file");
		fclose(fp);
		return(0);
	}


	// Search and get the data chunk
	struct {
		char	id[4];
		int		size;
	} WAV_Subchunk2;

	while(1) {

		fread(&WAV_Subchunk2, 1, sizeof(WAV_Subchunk2), fp);
		gdl::wii::RevBytes(&WAV_Subchunk2.size, sizeof(int));

		if (memcmp(&WAV_Subchunk2.id, "data", 4)) {
			fseek(fp, WAV_Subchunk2.size, SEEK_CUR);
		} else {
			break;
		}

	}


	// If all is valid, allocate memory for storing the wave data
	if (sData != NULL) vfree(sData);	// If already allocated, deallocate it for reallocation
	sData	= valloc(WAV_Subchunk2.size);
	sSize	= WAV_Subchunk2.size;
	freq	= WAV_Subchunk1.freq;


	// Load the sound data, flush it, and then close
	memset(sData, 0x00, sSize);
	fread(sData, 1, sSize, fp);
	fclose(fp);


	// Set audio format
	if (WAV_Subchunk1.chan == 1) {

		// For mono sounds
		switch(WAV_Subchunk1.bps) {
			case 8:

				format = VOICE_MONO_8BIT_U;
				break;

			case 16:

				format = VOICE_MONO_16BIT_LE;
				break;

		}

	} else {

		u_short buff;

		// For stereo sounds
		switch(WAV_Subchunk1.bps) {
			case 8:

				format = VOICE_STEREO_8BIT_U;

				for(int s=0; s<sSize; s+=2) {	// Swap stereo samples
					buff = ((u_char*)sData)[s];
					((u_char*)sData)[s] = ((u_char*)sData)[s+1];
					((u_char*)sData)[s+1] = buff;
				}

				break;

			case 16:

				format = VOICE_STEREO_16BIT_LE;

				for(int s=0; s<sSize/2; s+=2) {	// Swap stereo samples
					buff = ((u_short*)sData)[s];
					((u_short*)sData)[s] = ((u_short*)sData)[s+1];
					((u_short*)sData)[s+1] = buff;
				}

				break;

		}

	}

	DCFlushRange(sData, sSize);


	if (gdl::ConsoleActive)
		printf("Sound loaded Ok!\n");

	return(true);

}


void gdl::SoundWii::UnloadData() {

	if (sData != nullptr)
		vfree(sData);

	sData = nullptr;

}

void gdl::SoundWii::Play(float pitch, float volume) {

	// Simple play function

	if (sData == nullptr)
		return;

	voiceNumber = ASND_GetFirstUnusedVoice();
	if (voiceNumber == SND_INVALID)
		return;

	ASND_Pause(0);
	ASND_SetVoice(voiceNumber, format, freq*pitch, 0,
		sData, sSize, volume*((float)gdl::wii::MasterSfxVolume/100.f), volume*((float)gdl::wii::MasterSfxVolume/100.f), NULL);

}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void gdl::SoundWii::Play2D(float pitch, float volume, float x, float y) {

	if (sData == nullptr)
		return;

	short tslot = ASND_GetFirstUnusedVoice();
	if (tslot == SND_INVALID)
		return;

	// Calculate volumes according to the X position of the sound
	float lvol=1,rvol=1;
	if (x < 0) {
		lvol = 1.f;
		rvol = 1.f-((float)abs(x)/(gdl::ScreenXres*0.7f));
	} else if (x > 0) {
		lvol = 1.f-((float)x/(gdl::ScreenXres*0.7f));
		rvol = 1.f;
	}

	ASND_Pause(0);
	ASND_SetVoice(tslot, format, freq*pitch, 0,
		sData, sSize, (volume*((float)gdl::wii::MasterSfxVolume/100.f))*lvol, (volume*((float)gdl::wii::MasterSfxVolume/100.f))*rvol, NULL);

}

#pragma GCC diagnostic pop

float gdl::SoundWii::GetElapsedSeconds()
{
	if (voiceNumber != SND_INVALID)
	{
		return (float)ASND_GetTimerVoice(voiceNumber)/1000.0f;
	}
	else
	{
		return 0.0f;
	}
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void gdl::SoundWii::SetElapsedSeconds(float elapsed)
{
	// NOP
}
#pragma GCC diagnostic pop

gdl::SoundStatus gdl::SoundWii::GetStatus()
{
	s32 status = ASND_StatusVoice(voiceNumber);
	if (status == SND_WORKING) {
		return gdl::SoundStatus::Playing;
	}
	else if (status == SND_UNUSED)
	{
		if (sData == nullptr)
		{
			return gdl::SoundStatus::Unloaded;
		}
		else
		{
			return gdl::SoundStatus::Stopped;
		}
	}
	else
	{
		return gdl::SoundStatus::Paused;
	}
}

void gdl::SoundWii::SetPaused(bool setPaused)
{
	if (voiceNumber != SND_INVALID)
	{
		ASND_PauseVoice(voiceNumber, setPaused ? 1 : 0);
	}
}

void gdl::SoundWii::Stop()
{
	if (voiceNumber != SND_INVALID)
	{
		ASND_StopVoice(voiceNumber);
	}
}

// Sound system functions

void gdl::SetMasterVolumes(float musicVol, float soundVol) {

    gdl::wii::MasterMusicVolume  = musicVol;
    gdl::wii::MasterSfxVolume    = soundVol;

	SetVolumeOgg(255*((gdl::wii::UserMusicVolume*(gdl::wii::MasterMusicVolume/100.f))/100.f));

}


// Music functions

void gdl::SetMusicVolume(float volume) {

	gdl::wii::UserMusicVolume = volume;
    SetVolumeOgg(255*((gdl::wii::UserMusicVolume*(gdl::wii::MasterMusicVolume/100.f))/100.f));

}

// muffintrap: Added functions of the Music class
gdl::Music::Music()
{
	oggBuffer = NULL;
	bufferSize = 0;
}

gdl::Music::~Music()
{
	if (oggBuffer != NULL)
	{
		free(oggBuffer);
		oggBuffer = NULL;
	}
}

bool gdl::Music::LoadFromBuffer(const uint8_t* buffer, size_t size)
{
	bufferSize = size;
	oggBuffer = (uint8_t*)malloc(size);
	gdl_assert((oggBuffer != nullptr), "Could not allocate buffer for music");
	memcpy(oggBuffer, buffer, size);
	DCFlushRange(oggBuffer, size);
	return true;
}

bool gdl::Music::Play(bool loop)
{
	if (StatusOgg() == OGG_STATUS_RUNNING)
	{
		return false;
	}
	int playMode = OGG_ONE_TIME;
	if (loop) {
		playMode = OGG_INFINITE_TIME;
	}
	FILE* file = fmemopen(oggBuffer, bufferSize, "r");
	gdl_assert((file != nullptr), "Could not open music buffer as file");
	return (PlayOggFilePtr(file, 0, playMode) == 0);
}

float gdl::Music::GetElapsed()
{
	return (float)GetTimeOgg()/1000.0f;
}

void gdl::Music::JumpToSeconds(float seconds)
{
	SetTimeOgg(s32(seconds*1000.0f));
}

void gdl::Music::TogglePauseMusic()
{
	int pauseValue = 1;
	if (StatusOgg() == OGG_STATUS_PAUSED)
	{
		pauseValue = 0;
	}
	PauseOgg(pauseValue);
}

void gdl::Music::Stop()
{
	StopOgg();
}

// Added for 0.100.0-muffintrap: added a version that reads from a buffer.
// TODO: Python tool or some other way to strip const from the
// headers generated by bin2s
bool gdl::PlayMusic(u_char* buffer, size_t size, bool loop)
{
	// fmemopen cannot read from const buffer :U
	// Copy data to temporary buffer before reading
	/*
	if (oggTempBuffer != NULL)
	{
		free(oggTempBuffer);
	}
	oggTempBuffer = malloc(size);
	memcpy(oggTempBuffer, buffer, size);
	*/
	FILE* file = fmemopen(buffer, size, "r");
	bool result = PlayMusic(file, loop);
	return result;
}

// Added for 0.100.0-muffintrap: separated PlayMusic into two parts to avoid copying code
bool gdl::PlayMusic(const char* fileName, bool loop)
{
	FILE* file = fopen(fileName, "r");
	bool result = PlayMusic(file, loop);
	if (result)
	{
		gdl::wii::LastMusicFile = fileName;	// Save for possible later use
	}
	else {
		if ((StatusOgg() == OGG_STATUS_ERR) || (StatusOgg() == OGG_STATUS_EOF)) {

			if (PlayOgg(gdl::wii::LastMusicFile, 0, loop) != 0)
				return(false);
		}
	}
	return result;
}

// muffintrap: NOTE  oggplayer.c will close the file access.
bool gdl::PlayMusic(FILE* file, bool loop)
{
    if (file != NULL) {
		int playMode = OGG_ONE_TIME;
		if (loop) {
			playMode = OGG_INFINITE_TIME;
		}

		if (PlayOggFilePtr(file, 0, playMode) != 0)
			return(false);

		SetVolumeOgg(255*((gdl::wii::UserMusicVolume*(gdl::wii::MasterMusicVolume/100.f))/100.f));
		return(true);

    } 
	else {
		return false;
	}
}

void gdl::PauseMusic() {

    PauseOgg(1);

}

void gdl::StopMusic() {

    StopOgg();

}

#endif


#endif // _GDL_WII_SOUND
