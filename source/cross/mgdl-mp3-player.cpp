#define DR_MP3_IMPLEMENTATION
#include <mgdl/mgdl-mp3-player.h>


#include <mgdl/mgdl-logger.h>

static MusicMp3* musics;

#ifdef GEKKO
#include <valloc.h>
#include <stdio.h>
#include <mgdl/mgdl-cache.h>
static FILE* mp3FilePtr = nullptr;
static void* mp3FileBuffer = nullptr;
static sizetype mp3FileSize = 0;
#endif

// Called by Audio
static void Mp3_Callback(s32 voiceNumber, void* bufferPtr, u32 bufferSizeBytes, u32* bytesWritten)
{

	MusicMp3* m = &musics[voiceNumber];
	drmp3* mp3 = m->mp3;
	// Copy at max bufferSizeBytes of samples to buffer
	// One frame of audio is  sizeof(s16) * channels
	// For stereo sound, one frame is 4 bytes: 2 * 2
	// if buffer size is 16 bytes, we can decode 4 frames

	const int sizeOfs16 = 2;
	int bytesPerFrame = (mp3->channels * sizeOfs16);
	drmp3_uint64 framesToRead = bufferSizeBytes / bytesPerFrame;
	drmp3_uint64 framesRead = drmp3_read_pcm_frames_s16(mp3, framesToRead, (drmp3_int16*)bufferPtr);

	// One frame contains 1 sample. Divide by samples per second to get elapsed seconds
	m->secondsElapsed = (float)(m->framesElapsed)/ (float)mp3->sampleRate;
	m->framesElapsed += framesRead;
	//Log_InfoF("Mp3 callback: Elapsed frames %lu Elapsed seconds %.2f\n", m->framesElapsed, m->secondsElapsed);

	// If read less than requested, loop to start
	if (framesRead < framesToRead)
	{
		drmp3_seek_to_pcm_frame(mp3, 0);
		m->framesElapsed = 0;
		m->secondsElapsed = 0.0f;
	}

	(*bytesWritten) = framesRead * bytesPerFrame;
}

void Mp3Player_Init()
{
	musics = (MusicMp3*)malloc(sizeof(MusicMp3) * MGDL_AUDIO_MAX_SOUNDS);
	for(int i = 0; i < MGDL_AUDIO_MAX_SOUNDS; i++)
	{
		musics[i].mp3 = nullptr;
		musics[i].framesElapsed = 0;
	}
}
void Mp3Player_Deinit()
{
	for(int i = 0; i < MGDL_AUDIO_MAX_SOUNDS; i++)
	{
		if (musics[i].mp3 != nullptr)
		{
			drmp3_uninit(musics[i].mp3);
			free(musics[i].mp3);
		}
	}
	free(musics);
#	if defined(GEKKO)
	if (mp3FileBuffer != nullptr)
	{
		vfree(mp3FileBuffer);
	}
#	endif
}
Sound Mp3Player_LoadSound(const char* filename)
{
	Log_InfoF("Loading Mp3 from %s\n", filename);
	Sound s;
	Sound_InitEmpty(&s);
	// find first musics
	for(int i = 0; i < MGDL_AUDIO_MAX_SOUNDS; i++)
	{
		if (musics[i].mp3 == nullptr)
		{
			s.voiceNumber = i;
			break;
		}
	}
	MusicMp3* music = &musics[s.voiceNumber];
	music->mp3 = (drmp3*)malloc(sizeof(drmp3));
	bool loadOk = false;
#	if defined(GEKKO)
	// On Wii, read the file in fully.
	mp3FilePtr = fopen(filename, "r");
	if (mp3FilePtr != nullptr)
	{
		// Read until you figure out how big this file is
		fseek(mp3FilePtr, 0L, SEEK_END);
		mp3FileSize = ftell(mp3FilePtr);
		mp3FileBuffer = valloc(mp3FileSize);
		fseek(mp3FilePtr, 0L, SEEK_SET);
		fread(mp3FileBuffer, 1, mp3FileSize, mp3FilePtr);
		fclose(mp3FilePtr);

		mgdl_CacheFlushRange(mp3FileBuffer, mp3FileSize);

		loadOk = drmp3_init_memory(music->mp3, mp3FileBuffer, mp3FileSize, NULL);
	}
#	else
	loadOk = drmp3_init_file(music->mp3, filename, NULL);
#	endif

	if (loadOk)
	{
#		if defined(GEKKO)
		music->fileBuffer = mp3FileBuffer;
		music->sizeBytes = mp3FileSize;
#		endif
		// No idea how much is really allocated.
		music->sizeBytes = 1;
		s.type = SoundMp3;
		drmp3_uint64 pcmFrameAmount = drmp3_get_pcm_frame_count(music->mp3);

		float seconds= (float)pcmFrameAmount/ (float)music->mp3->sampleRate;
		Log_InfoF("Duration:%.2fs Samplerate:%u Channels:%u \n", seconds, music->mp3->sampleRate, music->mp3->channels);
		Log_InfoF("Mp3 loaded\n");
	}
	else
	{
		Sound_InitEmpty(&s);
		Log_Error("Failed to load Mp3");
	}

	return s;

}
void Mp3Player_PlaySound(Sound* snd)
{
	SoundSampleFormat format = Format_Stereo_16;
	if (musics[snd->voiceNumber].mp3->channels == 2)
	{
		format = Format_Stereo_16;
	}
	else if (musics[snd->voiceNumber].mp3->channels == 1)
	{
		format = Format_Mono_16;
	}
	else
	{
		Log_ErrorF("Cannot play Mp3 files with more than 2 channels");
	}
	Audio_Platform_SetCallback(Mp3_Callback);
	Audio_Platform_StartStream(snd, musics[snd->voiceNumber].mp3->sampleRate, format);
	musics[snd->voiceNumber].state = Audio_StatePlaying;
}

void Mp3Player_PauseSound(Sound* snd)
{
	Audio_Platform_PauseStream(snd);
	musics[snd->voiceNumber].state = Audio_StatePaused;
}
void Mp3Player_ResumeSound(Sound* snd)
{
	Audio_Platform_ResumeStream(snd);
	musics[snd->voiceNumber].state = Audio_StatePlaying;
}
void Mp3Player_StopSound(Sound* snd)
{
	Audio_Platform_StopStream(snd);
	musics[snd->voiceNumber].state = Audio_StateStopped;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
sizetype Mp3Player_GetSoundSizeBytes(Sound* snd)
{
	return musics[snd->voiceNumber].sizeBytes;
}
#pragma GCC diagnostic pop

u32 Mp3Player_GetSoundElapsedMs(Sound* snd)
{
	return musics[snd->voiceNumber].secondsElapsed*1000.0f;

}
void Mp3Player_SetSoundElapsedMs(Sound* snd, u32 milliseconds)
{
	s32 voiceNumber = snd->voiceNumber;
	MusicMp3* m = &musics[voiceNumber];

	drmp3_uint64 targetFrame = ((milliseconds * m->mp3->sampleRate)/1000);

	drmp3_seek_to_pcm_frame(m->mp3, targetFrame);
}

mgdlAudioStateEnum Mp3Player_GetSoundStatus(Sound* snd)
{
	return musics[snd->voiceNumber].state;

}
