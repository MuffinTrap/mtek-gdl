#include <mgdl/mgdl-audio.h>
#include <mgdl/mgdl-sound.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-ogg-player.h>
#include <mgdl/mgdl-wav-player.h>
#include <mgdl/mgdl-mp3-player.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-memory.h>


// TODO these are not used
static float MasterSfxVolume = 0;
static float MasterMusicVolume = 0;
static float UserMusicVolume = 0;


static Sound* sounds = nullptr;
static int firstFreeVoice = 0;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void Audio_Init(void* platformData)
{
    sounds = (Sound*)mgdl_AllocateGraphicsMemory(sizeof(struct Sound) * MGDL_AUDIO_MAX_SOUNDS);
    for (int i = 0; i < MGDL_AUDIO_MAX_SOUNDS; i++)
    {
		Sound_InitEmpty(&sounds[i]);
    }

    // TODO save memory by only initializing some players
    Audio_Platform_Init(platformData);
	WavPlayer_Init();
	OggPlayer_Init();
	Mp3Player_Init();
	firstFreeVoice = 0;

}
#pragma GCC diagnostic pop

void Audio_Deinit()
{
	Audio_Platform_Deinit();
	WavPlayer_Deinit();
	OggPlayer_Deinit();
	Mp3Player_Deinit();

    for (int i = 0; i < MGDL_AUDIO_MAX_SOUNDS; i++)
    {
		Sound_InitEmpty(&sounds[i]);
    }
	free(sounds);
	sounds = nullptr;
	firstFreeVoice = 0;
}

Sound* Sound_Load(const char* filename, SoundFileType filetype)
{
	// Find first voice that is not in use
	int loadedIndex = firstFreeVoice;
	if (firstFreeVoice >= MGDL_AUDIO_MAX_SOUNDS)
	{
		Log_Error("Cannot load any more sounds, all voices in use");
		return nullptr;
	}
	Sound s;
	switch(filetype)
	{
		case SoundWav:
		{
			s = WavPlayer_LoadSound(filename);
		}
		break;
		case SoundOgg:
		{
			s = OggPlayer_LoadSound(filename);
		}
		break;
		case SoundMp3:
		{
			s = Mp3Player_LoadSound(filename);
		}
		break;
	}

	if (s.voiceNumber < 0)
	{
		// Failed to load
		Log_ErrorF("Failed to load sound from file %s\n", filename);
		return nullptr;
	}
	else {
		Log_InfoF("Loaded sound to slot %d\n", loadedIndex);
	}

	s.type = filetype;
	sounds[loadedIndex] = s;
	firstFreeVoice += 1;

	return &sounds[loadedIndex];
}

void Sound_Play(Sound* s)
{
	ASSERT_DEBUG(s!=nullptr);
	switch(s->type)
	{
		case SoundWav:
		{
			WavPlayer_PlaySound(s);
		}
		break;
		case SoundOgg:
		{
			OggPlayer_PlaySound(s);
		}
		break;
		case SoundMp3:
		{
			Mp3Player_PlaySound(s);
		}
		break;
	}
}
void Sound_Pause(Sound* s)
{
	ASSERT_DEBUG(s!=nullptr);
	switch(s->type)
	{
		case SoundWav:
		{
			WavPlayer_PauseSound(s);
		}
		break;
		case SoundOgg:
		{
			OggPlayer_PauseSound(s);
		}
		break;
		case SoundMp3:
		{
			Mp3Player_PauseSound(s);
		}
		break;
	}
}
void Sound_Resume(Sound* s)
{
	ASSERT_DEBUG(s!=nullptr);
	switch(s->type)
	{
		case SoundWav:
		{
			WavPlayer_ResumeSound(s);
		}
		break;
		case SoundOgg:
		{
			OggPlayer_ResumeSound(s);
		}
		break;
		case SoundMp3:
		{
			Mp3Player_ResumeSound(s);
		}
		break;
	}
}
void Sound_Stop(Sound* s)
{
	ASSERT_DEBUG(s!=nullptr);
	switch(s->type)
	{
		case SoundWav:
		{
			WavPlayer_StopSound(s);
		}
		break;
		case SoundOgg:
		{
			OggPlayer_StopSound(s);
		}
		break;
		case SoundMp3:
		{
			Mp3Player_StopSound(s);
		}
		break;
	}
}

sizetype Audio_GetSoundSizeBytes(Sound* snd)
{
	ASSERT_DEBUG(snd!=nullptr);
	switch (snd->type)
	{
	case SoundWav:
		return WavPlayer_GetSoundSizeBytes(snd);
		break;
	case SoundOgg:
		return OggPlayer_GetSoundSizeBytes(snd);
		break;
	case SoundMp3:
		return Mp3Player_GetSoundSizeBytes(snd);
		break;
	}
	return 0;
}
u32 Sound_GetElapsedMs(Sound* snd)
{
	ASSERT_DEBUG(snd!=nullptr);
	switch (snd->type)
	{
	case SoundWav:
		return WavPlayer_GetSoundElapsedMs(snd);
		break;
	case SoundOgg:
		return OggPlayer_GetSoundElapsedMs(snd);
		break;
	case SoundMp3:
		return Mp3Player_GetSoundElapsedMs(snd);
		break;
	}
	return 0;

}

mgdlAudioStateEnum Sound_GetStatus(Sound* snd)
{
	ASSERT_DEBUG(snd!=nullptr);
	switch (snd->type)
	{
	case SoundWav:
		return Audio_GetStaticBufferStatus(snd);
		break;
	case SoundOgg:
		return OggPlayer_GetSoundStatus(snd);
		break;
	case SoundMp3:
		return Mp3Player_GetSoundStatus(snd);
		break;
	}
	return Audio_StateInvalid;

}
// Sound system functions

void Sound_SetElapsedMs(Sound* snd, s32 milliseconds)
{
	ASSERT_DEBUG(snd!=nullptr);
	switch (snd->type)
	{
	case SoundWav:
		return WavPlayer_SetSoundElapsedMs(snd, milliseconds);
		break;
	case SoundOgg:
		return OggPlayer_SetSoundElapsedMs(snd, milliseconds);
		break;
	case SoundMp3:
		return Mp3Player_SetSoundElapsedMs(snd, milliseconds);
		break;
	}
}

void Sound_SetVolume(Sound* snd, float normalizedVolume)
{
	ASSERT_DEBUG(snd!=nullptr);

	normalizedVolume = Clamp(normalizedVolume, 0.0f, 1.0f);
	switch (snd->type)
	{
	case SoundWav:
		return WavPlayer_SetSoundVolume(snd, normalizedVolume);
		break;
	case SoundOgg:
		return OggPlayer_SetSoundVolume(snd, normalizedVolume);
		break;
	case SoundMp3:
		return Mp3Player_SetSoundVolume(snd, normalizedVolume);
		break;
	}
	snd->normalizedVolume = normalizedVolume;
}

void Audio_SetSoundMasterVolume(float normalizedVolume)
{
    MasterSfxVolume = normalizedVolume;
}

void Audio_SetMusicMasterVolume(float normalizedVolume)
{
    MasterMusicVolume  = normalizedVolume;
}


// Music functions

void Audio_SetUserMusicVolume(float normalizedVolume)
{
	UserMusicVolume = normalizedVolume;
}

