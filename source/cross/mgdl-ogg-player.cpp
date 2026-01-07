#include <mgdl/mgdl-ogg-player.h>
#include <mgdl/mgdl-audio.h>
#include <mgdl/mgdl-sound.h>
#include <mgdl/mgdl-music.h>
#include <mgdl/mgdl-logger.h>
static MusicOgg* musics = nullptr;

#ifndef GEKKO
#include "../source/stb/stb_vorbis.c"


static void Silent_Callback(s32 voiceNumber, s16* bufferPtr, u32 bufferSizeBytes, u32* bytesWritten)
{
	memset(bufferPtr, 0, bufferSizeBytes);
	(*bytesWritten) = bufferSizeBytes;
}

static void Ogg_Callback(s32 voiceNumber, s16* bufferPtr, u32 bufferSizeBytes, u32* bytesWritten)
{
	stb_vorbis* vorbisfile = musics[voiceNumber].vorbisfile;
	int channels = musics[voiceNumber].channels;
	int num_shorts = bufferSizeBytes / 2;
	int samplesWritten = stb_vorbis_get_samples_short_interleaved(vorbisfile, channels, bufferPtr, num_shorts);
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
		stb_vorbis_seek_start(vorbisfile);
	}
}

static MusicOgg LoadOgg(MusicOgg m, Sound* inout_snd, const char* filename, s32 voiceNumber)
{
	Log_InfoF("Loading Ogg Sound to DirectSound from %s\n", filename);
	int errorOut = 0;
	stb_vorbis* vorbisFile = stb_vorbis_open_filename(filename, &errorOut, NULL);
	if (vorbisFile == nullptr)
	{
		switch (errorOut)
		{

		}
		Log_ErrorF("Failed to open ogg file %s\n", filename);
		return m;
	}
	stb_vorbis_info oggInfo = stb_vorbis_get_info(vorbisFile);
	stb_vorbis_seek_start(vorbisFile);

	m.lengthSeconds = stb_vorbis_stream_length_in_seconds(vorbisFile);
	m.lengthSamples = stb_vorbis_stream_length_in_samples(vorbisFile);
	m.vorbisfile = vorbisFile;
	Sound_Init(inout_snd, voiceNumber, SoundOgg);

	Log_InfoF("Opened Ogg stream: sample rate: %u, channels: %d, mem: %d + %d + %d, max frame size %d\n", oggInfo.sample_rate, oggInfo.channels, oggInfo.setup_memory_required, oggInfo.setup_temp_memory_required, oggInfo.temp_memory_required, oggInfo.max_frame_size);
	Log_InfoF("Length %.2f seconds, %d samples\n", m.lengthSeconds, m.lengthSamples);
	m.channels = oggInfo.channels;
	m.elapsedSeconds = 0.0f;
	m.state = Audio_StateStopped;

	return m;
}
static s16* testBuffer = nullptr;

static void TestOgg(s32 voice, int cycles)
{
	testBuffer = (s16*)malloc(MGDL_AUDIO_CALLBACK_BUFFER_SIZE);
	u32 outb;
	for (int i = 0; i < cycles; i++)
	{
		Ogg_Callback(voice, testBuffer, MGDL_AUDIO_CALLBACK_BUFFER_SIZE, &outb);
	}
}

void OggPlayer_Init()
{
    musics = (MusicOgg*)malloc(sizeof(struct MusicOgg) * MGDL_AUDIO_MAX_MUSIC);
    for (int i = 0; i < MGDL_AUDIO_MAX_MUSIC; i++)
    {
		musics[i].vorbisfile = nullptr;
    }
}

void OggPlayer_Deinit()
{
    for (int i = 0; i < MGDL_AUDIO_MAX_MUSIC; i++)
    {
		stb_vorbis_close(musics[i].vorbisfile);
    }
	free(musics);
	musics = nullptr;
}

Sound OggPlayer_LoadSound(const char* filename)
{
	Sound snd;
	Sound_InitEmpty(&snd);
	// Find first music that is not in use
	for (int i = 0; i < MGDL_AUDIO_MAX_MUSIC; i++)
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
	//TestOgg(s->voiceNumber, 5);
	Audio_Platform_SetCallback(Ogg_Callback);
	Audio_Platform_StartStream(snd, musics[snd->voiceNumber].sampleRate);
	musics[snd->voiceNumber].state = Audio_StatePlaying;
}

sizetype OggPlayer_GetSoundSizeBytes(Sound* snd)
{
	return musics[snd->voiceNumber].sizeBytes;
}

u32 OggPlayer_GetSoundElapsedMs(Sound* snd)
{
	return musics[snd->voiceNumber].elapsedSeconds * 1000;
}

mgdlAudioStateEnum OggPlayer_GetSoundStatus(Sound* snd)
{
	return musics[snd->voiceNumber].state;
}
#endif
