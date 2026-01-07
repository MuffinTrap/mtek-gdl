#include <mgdl/mgdl-wav-player.h>
#include <mgdl/mgdl-audio.h>
#include <mgdl/mgdl-sound.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-util.h>

#include <cstdio>

void WavPlayer_Init()
{
}
void WavPlayer_Deinit()
{
}

Sound WavPlayer_LoadSound(const char* filename)
{
	Sound sound;
	Sound_InitEmpty(&sound);
	// Open up the wave file
	Log_InfoF("Opening sound file %s\n", filename);

	FILE *fp = fopen(filename, "r");

	if (fp == nullptr)
	{
		Log_ErrorF("\tSound file %s not found", filename);
		return sound;
	}

	// Get header chunk
	struct {
		char	id[4];
		int		size;
		char	format[4];
	} WAV_Chunk;

	fread(&WAV_Chunk, 1, sizeof(WAV_Chunk), fp);

	if (memcmp(&WAV_Chunk.id, "RIFF", 4) || memcmp(&WAV_Chunk.format, "WAVE", 4)) {
		Log_ErrorF("Sound file is not a Microsoft WAVE file");
		fclose(fp);
		return sound;
	}

#ifdef GEKKO
	RevBytes(&WAV_Chunk.size, sizeof(int));
#endif

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

#ifdef GEKKO
	// Convert values we need into BE
	RevBytes(&WAV_Subchunk1.size, sizeof(int));
	RevBytes(&WAV_Subchunk1.format, sizeof(short));
	RevBytes(&WAV_Subchunk1.chan, sizeof(short));
	RevBytes(&WAV_Subchunk1.freq, sizeof(int));
	RevBytes(&WAV_Subchunk1.bps, sizeof(short));
#endif

	if (memcmp(&WAV_Subchunk1.id, "fmt ", 4)) {
		Log_ErrorF("Unsupported WAV format variant in sound file");
		fclose(fp);
		return sound;
	}


	// Search and get the data chunk
	struct {
		char	id[4];
		int		size;
	} WAV_Subchunk2;

	while(true) {

		fread(&WAV_Subchunk2, 1, sizeof(WAV_Subchunk2), fp);

#ifdef GEKKO
		RevBytes(&WAV_Subchunk2.size, sizeof(int));
#endif

		if (memcmp(&WAV_Subchunk2.id, "data", 4)) {
			fseek(fp, WAV_Subchunk2.size, SEEK_CUR);
		} else {
			break;
		}

	}

	SoundSampleFormat format;

	// Set audio format
	if (WAV_Subchunk1.chan == 1) {
		// For mono sounds
		switch (WAV_Subchunk1.bps) {
		case 8:
			format = Sound_Mono_s8;
			break;
		case 16:
			format = Sound_Mono_s16;
			break;
		}
	}
	else {
		// For stereo sounds
		switch (WAV_Subchunk1.bps) {
		case 8:
			format = Sound_Stereo_s8;
			break;
		case 16:
			format = Sound_Stereo_s16;
			break;
		}
	}
	sizetype byteAmount = WAV_Subchunk2.size;
	u16 samplerate	= WAV_Subchunk1.freq;


	void* audioBuffer = Audio_OpenStaticBuffer(&sound, byteAmount, samplerate, format);

	// Load the sound data, flush it, and then close
	fread(audioBuffer, 1, byteAmount, fp);
	fclose(fp);

	Audio_CloseStaticBuffer(&sound, audioBuffer, byteAmount);

	Log_Info("\tSound loaded Ok!\n");
	sound.type = SoundWav;

	return sound;
}

void WavPlayer_PlaySound(Sound* snd)
{
	Audio_PlayStaticBuffer(snd);
}

sizetype WavPlayer_GetSoundSizeBytes(Sound* snd) 
{
	return Audio_GetStaticBufferSize(snd);

}

u32 WavPlayer_GetSoundElapsedMs(Sound* snd)
{
	return Audio_GetStaticBufferElapsedMs(snd);
}
