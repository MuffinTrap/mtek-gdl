// Sound class module functions

#ifdef GEKKO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gccore.h>
#include <asndlib.h>
#include <oggplayer.h>

#include "mgdl/wii/mgdl-wii-config.h"
#include "mgdl/wii/mgdl-wii-globals.h"
#include "mgdl/wii/mgdl-wii-globals-internal.h"
#include "mgdl/wii/mgdl-wii-main.h"
#include "mgdl/wii/mgdl-wii-sound.h"
#include "mgdl/wii/mgdl-wii-assert.h"



Sound* Sound_Load(const char* fileName) {

	// Open up the wave file
	Log_InfoF("Opening sound file %s\n", fileName);


	FILE *fp = fopen(fileName, "r");

	if (fp == nullptr)
	{
		gdl::CallErrorCallback("\tSound file %s not found", fileName);
		return nullptr;
	}

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
		return nullptr;
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
		return nullptr;
	}


	// Search and get the data chunk
	struct {
		char	id[4];
		int		size;
	} WAV_Subchunk2;

	while(true) {

		fread(&WAV_Subchunk2, 1, sizeof(WAV_Subchunk2), fp);
		gdl::wii::RevBytes(&WAV_Subchunk2.size, sizeof(int));

		if (memcmp(&WAV_Subchunk2.id, "data", 4)) {
			fseek(fp, WAV_Subchunk2.size, SEEK_CUR);
		} else {
			break;
		}

	}

	Sound* sound = new Sound();

	// If all is valid, allocate memory for storing the wave data
	if (sound->sData != NULL) { vfree(sound->sData); } // If already allocated, deallocate it for reallocation
	sound->sData	= valloc(WAV_Subchunk2.size);
	sound->sSize	= WAV_Subchunk2.size;
	sound->freq	= WAV_Subchunk1.freq;


	// Load the sound data, flush it, and then close
	memset(sound->sData, 0x00, sound->sSize);
	fread(sound->sData, 1, sound->sSize, fp);
	fclose(fp);


	// Set audio format
	if (WAV_Subchunk1.chan == 1) {

		// For mono sounds
		switch(WAV_Subchunk1.bps) {
			case 8:

				sound->format = VOICE_MONO_8BIT_U;
				break;

			case 16:

				sound->format = VOICE_MONO_16BIT_LE;
				break;

		}

	} else {

		u_short buff;

		// For stereo sounds
		switch(WAV_Subchunk1.bps) {
			case 8:

				sound->format = VOICE_STEREO_8BIT_U;

				for(sizetype s=0; s<sound->sSize; s+=2) {	// Swap stereo samples
					buff = ((u_char*)sound->sData)[s];
					((u_char*)sound->sData)[s] = ((u_char*)sound->sData)[s+1];
					((u_char*)sound->sData)[s+1] = buff;
				}

				break;

			case 16:

				sound->format = VOICE_STEREO_16BIT_LE;

				for(sizetype s=0; s<sound->sSize/2; s+=2) {	// Swap stereo samples
					buff = ((u_short*)sound->sData)[s];
					((u_short*)sound->sData)[s] = ((u_short*)sound->sData)[s+1];
					((u_short*)sound->sData)[s+1] = buff;
				}

				break;

		}

	}

	DCFlushRange(sound->sData, sound->sSize);

	Log_Info("\tSound loaded Ok!\n");

	return sound;
}

void Sound_Init(Sound* sound) {

	sound->format=0;
	sound->freq=0;
	sound->sData=NULL;
	sound->sSize=0;
	sound->voiceNumber = SND_INVALID;
}


void Sound_DeleteData(Sound* sound)
{
	if (sound->sData != nullptr)
	{
		vfree(sound->sData);
	}
	sound->sData = nullptr;
}

void Sound_Play(Sound* sound, float pitch, float volume) {

	// Simple play function
	if (sound->sData == nullptr)
		return;

	sound->voiceNumber = ASND_GetFirstUnusedVoice();
	if (sound->voiceNumber == SND_INVALID)
	{
		return;
	}

	ASND_Pause(0);
	ASND_SetVoice(
		sound->voiceNumber,
		sound->format,
		sound->freq*pitch,
		0,
		sound->sData,
		sound->sSize,
		volume*((float)gdl::wii::MasterSfxVolume/100.f),
		volume*((float)gdl::wii::MasterSfxVolume/100.f),
		NULL);
}

void Sound_Stop(Sound* sound)
{
	if (sound->voiceNumber != SND_INVALID)
	{
		ASND_StopVoice(sound->voiceNumber);
	}
}

void Sound_SetPaused(Sound* sound, bool setPaused)
{
	if (sound->voiceNumber != SND_INVALID)
	{
		ASND_PauseVoice(sound->voiceNumber, setPaused ? 1 : 0);
	}
}

void Sound_SetLooping(Sound* sound, bool looping)
{
	sound->isLooping = looping;
}

bool Sound_GetLooping(Sound* sound)
{
	return sound->isLooping;

}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void Sound_Play2D(Sound* sound, float pitch, float volume, float x, float y) {

	if (sound->sData == nullptr)
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
	ASND_SetVoice(
		tslot,
		sound->format,
		sound->freq*pitch,
		0,
		sound->sData,
		sound->sSize,
		(volume*((float)gdl::wii::MasterSfxVolume/100.f))*lvol,
		(volume*((float)gdl::wii::MasterSfxVolume/100.f))*rvol,
		NULL);
}

#pragma GCC diagnostic pop

float Sound_GetElapsedSeconds(Sound* sound)
{
	if (sound->voiceNumber != SND_INVALID)
	{
		return sound->secondsOffset + (float)ASND_GetTimerVoice(sound->voiceNumber)/1000.0f;
	}
	else
	{
		return 0.0f;
	}
}

void Sound_SetElapsedSeconds(Sound* sound, float elapsed)
{
	sound->secondsOffset = elapsed;
}

Sound_Status Sound_GetStatus(Sound* sound)
{
	s32 status = ASND_StatusVoice(sound->voiceNumber);
	if (status == SND_WORKING)
	{
		return Sound_Status::Playing;
	}
	else if (status == SND_UNUSED)
	{
		if (sound->sData == nullptr)
		{
			return Sound_Status::Initial;
		}
		else
		{
			return Sound_Status::Stopped;
		}
	}
	else
	{
		return Sound_Status::Paused;
	}
}

#endif // GEKKO
