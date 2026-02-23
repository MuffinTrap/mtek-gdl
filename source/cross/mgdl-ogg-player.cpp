#include <mgdl/mgdl-ogg-player.h>
#include <mgdl/mgdl-audio.h>
#include <mgdl/mgdl-alloc.h>
#include <mgdl/mgdl-sound.h>
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-cache.h>

#include <stdio.h>


static MusicOgg* musics = nullptr;
static FILE* oggFilePtr = nullptr;
static sizetype oggFileSize = 0;


// Wii uses big endian
#ifdef GEKKO
#	define STB_VORBIS_BIG_ENDIAN
#endif
#define STB_VORBIS_NO_PUSHDATA_API
#include "../source/stb/stb_vorbis.c"

static void Ogg_Callback(s32 voiceNumber, void* bufferPtr, u32 bufferSizeBytes, u32* bytesWritten)
{
	stb_vorbis* vorbisfile = musics[voiceNumber].vorbisfile;
	int channels = musics[voiceNumber].channels;
	int num_shorts = bufferSizeBytes / 2;
	int samplesWritten = stb_vorbis_get_samples_short_interleaved(vorbisfile, channels, (short*)bufferPtr, num_shorts);
	//Log_InfoF("Vorbis wrote %d samples per channel\n", samplesWritten);
	*bytesWritten = samplesWritten * 2 * channels;

	int samplesElapsed = stb_vorbis_get_sample_offset(vorbisfile);
	if (samplesElapsed > 0)
	{
		musics[voiceNumber].elapsedSeconds = (float)samplesElapsed / (float)musics[voiceNumber].sampleRate;
	}
	// Loop back to beginning when close to end
	if (*bytesWritten < bufferSizeBytes)
	{
		// Log_Info("Ogg stream almost at end, seek back to start\n");
		stb_vorbis_seek_start(vorbisfile);
	}
}

/* Callbacks for testing */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-function"
static void Silent_Callback(s32 voiceNumber, void* bufferPtr, u32 bufferSizeBytes, u32* bytesWritten)
{
	Ogg_Callback(voiceNumber, bufferPtr, bufferSizeBytes, bytesWritten);
	memset(bufferPtr, 0, bufferSizeBytes);
	(*bytesWritten) = bufferSizeBytes;
}

static void* testBuffer = nullptr;
static void TestOgg(s32 voice, int cycles)
{
	testBuffer = malloc(MGDL_AUDIO_CALLBACK_BUFFER_SIZE);
	u32 outb;
	for (int i = 0; i < cycles; i++)
	{
		Ogg_Callback(voice, testBuffer, MGDL_AUDIO_CALLBACK_BUFFER_SIZE, &outb);
	}
	free(testBuffer);
}
#pragma GCC diagnostic pop


static MusicOgg LoadOgg(MusicOgg m, Sound* inout_snd, const char* filename, s32 voiceNumber)
{
	Log_InfoF("Loading Ogg from %s\n", filename);
	int errorOut = 0;


	// Can let stb_vorbis to allocate memory or give it a pre allocated pointer
	stb_vorbis_alloc* ogg_allocation_ptr = nullptr;
	bool useCustomAllocation = false;
	if (useCustomAllocation)
	{
		stb_vorbis_alloc ogg_allocation;
		sizetype amount = 250 * 1024;
		ogg_allocation.alloc_buffer = (char*)mgdl_AllocateGraphicsMemory(amount);
		ogg_allocation.alloc_buffer_length_in_bytes = amount;
		ogg_allocation_ptr = &ogg_allocation;
	}

	// Read the file in fully
	stb_vorbis* vorbisFile = nullptr;
	void* oggFileBuffer = nullptr;
	oggFilePtr = fopen(filename, "r");
	if (oggFilePtr != nullptr)
	{
		// Read until you figure out how big this file is
		fseek(oggFilePtr, 0L, SEEK_END);
		oggFileSize = ftell(oggFilePtr);
		oggFileBuffer = mgdl_AllocateGeneralMemory(oggFileSize);
		fseek(oggFilePtr, 0L, SEEK_SET);
		fread(oggFileBuffer, 1, oggFileSize, oggFilePtr);
		fclose(oggFilePtr);
		mgdl_CacheFlushRange(oggFileBuffer, oggFileSize);
		vorbisFile = stb_vorbis_open_memory((const unsigned char*)oggFileBuffer, oggFileSize, &errorOut, ogg_allocation_ptr);
	}

	if (useCustomAllocation && errorOut == VORBIS_outofmem)
	{
		// Not enough memory supplied. Try again.
		Log_Error("Ogg Player needs more pre allocated memory to work!");
		return m;
	}
	if (vorbisFile == nullptr)
	{
		Log_ErrorF("Failed to open ogg file %s\n", filename);
		return m;
	}

	stb_vorbis_info oggInfo = stb_vorbis_get_info(vorbisFile);
	stb_vorbis_seek_start(vorbisFile);

	m.lengthSeconds = stb_vorbis_stream_length_in_seconds(vorbisFile);
	m.lengthSamples = stb_vorbis_stream_length_in_samples(vorbisFile);
	m.vorbisfile = vorbisFile;
	Sound_Init(inout_snd, voiceNumber, SoundOgg);
	
	m.fileBuffer = oggFileBuffer;
	m.sizeBytes = oggInfo.setup_memory_required + oggInfo.setup_temp_memory_required + oggInfo.temp_memory_required + oggFileSize;

	Log_InfoF("Duration:%.2fs SampleRate:%u Channels:%d\n", m.lengthSeconds, oggInfo.sample_rate, oggInfo.channels);
	m.channels = oggInfo.channels;
	m.elapsedSeconds = 0.0f;
	m.sampleRate = oggInfo.sample_rate;
	m.state = Audio_StateStopped;

	return m;
}


void OggPlayer_Init()
{
    musics = (MusicOgg*)malloc(sizeof(struct MusicOgg) * MGDL_AUDIO_MAX_SOUNDS);
    for (int i = 0; i < MGDL_AUDIO_MAX_SOUNDS; i++)
    {
		musics[i].vorbisfile = nullptr;
		musics[i].fileBuffer = nullptr;
    }
}

void OggPlayer_Deinit()
{
    for (int i = 0; i < MGDL_AUDIO_MAX_SOUNDS; i++)
    {
		stb_vorbis_close(musics[i].vorbisfile);
		if (musics[i].fileBuffer != nullptr)
		{
			mgdl_FreeGeneralMemory(musics[i].fileBuffer);
		}
    }
	free(musics);
	musics = nullptr;
}

Sound OggPlayer_LoadSound(const char* filename)
{
	Sound snd;
	Sound_InitEmpty(&snd);
	// Find first music that is not in use
	for (int i = 0; i < MGDL_AUDIO_MAX_SOUNDS; i++)
	{
		if (musics[i].vorbisfile == nullptr)
		{
			musics[i] = LoadOgg(musics[i], &snd, filename, i);
			snd.voiceNumber = i;
			break;
		}
	}
	if (snd.voiceNumber < 0)
	{
		Log_Error("Cannot load any more ogg musics, all slots in use");
	}
	return snd;
}

void OggPlayer_PlaySound(Sound* snd)
{
	SoundSampleFormat format = Format_Stereo_16;
	if (musics[snd->voiceNumber].channels == 2)
	{
		format = Format_Stereo_16;
	}
	else if (musics[snd->voiceNumber].channels == 1)
	{
		format = Format_Mono_16;
	}
	else
	{
		Log_ErrorF("Cannot play Ogg files with more than 2 channels");
	}
	Audio_Platform_SetCallback(Ogg_Callback);
	Audio_Platform_StartStream(snd, musics[snd->voiceNumber].sampleRate, format);
	musics[snd->voiceNumber].state = Audio_StatePlaying;
}

void OggPlayer_PauseSound(Sound* snd)
{
	if (musics[snd->voiceNumber].state == Audio_StatePlaying)
	{
		Audio_Platform_PauseStream(snd);
		musics[snd->voiceNumber].state = Audio_StatePaused;
	}
}
void OggPlayer_ResumeSound(Sound* snd)
{
	if (musics[snd->voiceNumber].state == Audio_StatePaused)
	{
		Audio_Platform_ResumeStream(snd);
		musics[snd->voiceNumber].state = Audio_StatePlaying;
	}
}

void OggPlayer_StopSound(Sound* snd)
{
	if (musics[snd->voiceNumber].state == Audio_StatePlaying)
	{
		Audio_Platform_StopStream(snd);
		musics[snd->voiceNumber].state = Audio_StateStopped;
	}
}

sizetype OggPlayer_GetSoundSizeBytes(Sound* snd)
{
	return musics[snd->voiceNumber].sizeBytes;
}

u32 OggPlayer_GetSoundElapsedMs(Sound* snd)
{
	return musics[snd->voiceNumber].elapsedSeconds * 1000;
}
void OggPlayer_SetSoundElapsedMs(Sound* snd, u32 milliseconds)
{
	unsigned int sample = 0;
	float secondsIn = (float)milliseconds / 1000.0f;
	sample = (musics[snd->voiceNumber].channels * musics[snd->voiceNumber].sampleRate) * secondsIn;
	stb_vorbis_seek(musics[snd->voiceNumber].vorbisfile, sample);
	musics[snd->voiceNumber].elapsedSeconds = secondsIn;
}

mgdlAudioStateEnum OggPlayer_GetSoundStatus(Sound* snd)
{
	return musics[snd->voiceNumber].state;
}

