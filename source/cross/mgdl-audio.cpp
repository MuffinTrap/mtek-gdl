#include <mgdl/mgdl-audio.h>
#include <mgdl/mgdl-sound.h>
#include <mgdl/mgdl-music.h>
#include <mgdl/mgdl-logger.h>


static float MasterSfxVolume = 0;
static float MasterMusicVolume = 0;
static float UserMusicVolume = 0;

static Sound* sounds = nullptr;
static MusicOgg* musics = nullptr;

// TODO Move to separate Ogg player that uses Audio system
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
	int channels = musics[voiceNumber].snd.channels;
	int num_shorts = bufferSizeBytes / 2;
	int samplesWritten = stb_vorbis_get_samples_short_interleaved(vorbisfile, channels, bufferPtr, num_shorts);
	//Log_InfoF("Vorbis wrote %d samples per channel\n", samplesWritten);
	*bytesWritten = samplesWritten * 2 * channels;

	int samplesElapsed = stb_vorbis_get_sample_offset(vorbisfile);
	if (samplesElapsed > 0)
	{
		musics[voiceNumber].snd.elapsedSeconds = (float)samplesElapsed / (float)musics[voiceNumber].sampleRate;
	}
	// Loop back to beginning when close to end
	if (*bytesWritten < bufferSizeBytes)
	{
		stb_vorbis_seek_start(vorbisfile);
	}
}

static MusicOgg LoadOgg(MusicOgg m, const char* filename, s32 voiceNumber)
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
	Sound_Init(&m.snd, voiceNumber, oggInfo.channels, oggInfo.setup_memory_required, SoundOgg);

	Log_InfoF("Opened Ogg stream: sample rate: %u, channels: %d, mem: %d + %d + %d, max frame size %d\n", oggInfo.sample_rate, oggInfo.channels, oggInfo.setup_memory_required, oggInfo.setup_temp_memory_required, oggInfo.temp_memory_required, oggInfo.max_frame_size);
	Log_InfoF("Length %.2f seconds, %d samples\n", m.lengthSeconds, m.lengthSamples);

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
#endif

void Audio_Init(void* platformData)
{
    Audio_Platform_Init(platformData);
    sounds = (Sound*)malloc(sizeof(struct Sound) * MGDL_AUDIO_MAX_VOICES);
    for (int i = 0; i < MGDL_AUDIO_MAX_VOICES; i++)
    {
		Sound_InitEmpty(&sounds[i]);
    }
    musics = (MusicOgg*)malloc(sizeof(struct MusicOgg) * MGDL_AUDIO_MAX_MUSIC);
    for (int i = 0; i < MGDL_AUDIO_MAX_MUSIC; i++)
    {
		Sound_InitEmpty(&musics[i].snd);
		musics[i].vorbisfile = nullptr;
    }
}

void Audio_Deinit()
{
	Audio_Platform_Deinit();
    for (int i = 0; i < MGDL_AUDIO_MAX_VOICES; i++)
    {
		Sound_InitEmpty(&sounds[i]);
    }
	free(sounds);
	sounds = nullptr;
    for (int i = 0; i < MGDL_AUDIO_MAX_MUSIC; i++)
    {
		stb_vorbis_close(musics[i].vorbisfile);
    }
	free(musics);
	musics = nullptr;
}

Sound* Audio_LoadSound(const char* filename, SoundFileType filetype)
{
	if (filetype == SoundWav)
	{
		Sound s = Audio_Platform_LoadSound(filename, filetype);
		if (s.sizeBytes <= 0)
		{
			// Failed to load
			Log_ErrorF("Failed to load sound from file %s", filename);
		}
		if (s.voiceNumber >= 0 && s.voiceNumber < MGDL_AUDIO_MAX_VOICES)
		{
			// Find first voice that is not in use
			for (int i = 0; i < MGDL_AUDIO_MAX_VOICES; i++)
			{
				if (sounds[i].sizeBytes <= 0)
				{
					s.type = SoundWav;
					sounds[i] = s;
					return &sounds[i];
				}
			}
		}
		Log_Error("Cannot load any more sounds, all voices in use");
	}
	else if (filetype == SoundOgg)
	{
		s32 voicenumber = -1;
		// Find first music that is not in use
		for (int i = 0; i < MGDL_AUDIO_MAX_MUSIC; i++)
		{
			if (musics[i].snd.sizeBytes <= 0)
			{
				musics[i] = LoadOgg(musics[i], filename, i);
				return &musics[i].snd;
			}
		}
		Log_Error("Cannot load any more musics, all slots in use");
	}
    return nullptr;
}

void Audio_PlaySound(Sound* s)
{
	if (s == nullptr)
	{
		return;
	}
	if (s->type == SoundWav)
	{
		if (s->voiceNumber >= 0 && s->voiceNumber < MGDL_AUDIO_MAX_VOICES)
		{
			Audio_Platform_PlaySound(s);
		}
	}
	else if (s->type == SoundOgg)
	{
		//TestOgg(s->voiceNumber, 5);
		Audio_Platform_SetCallback(Ogg_Callback);
		Audio_Platform_StartStream(s, musics[s->voiceNumber].sampleRate);
	}
}

// Sound system functions

void Audio_SetSoundMasterVolume(float normalizedVolume)
{
    MasterSfxVolume = normalizedVolume;
}

void Audio_SetMusicMasterVolume(float normalizedVolume)
{
    MasterMusicVolume  = normalizedVolume;
#ifdef GEKKO
	SetVolumeOgg(255*(UserMusicVolume*MasterMusicVolume));
#endif
}


// Music functions

void Audio_SetUserMusicVolume(float normalizedVolume)
{
	UserMusicVolume = normalizedVolume;
#ifdef GEKKO
    SetVolumeOgg(255*(UserMusicVolume*MasterMusicVolume));
#endif
}

