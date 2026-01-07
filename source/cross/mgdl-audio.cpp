#include <mgdl/mgdl-audio.h>
#include <mgdl/mgdl-sound.h>
#include <mgdl/mgdl-music.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-ogg-player.h>
#include <mgdl/mgdl-wav-player.h>


static float MasterSfxVolume = 0;
static float MasterMusicVolume = 0;
static float UserMusicVolume = 0;


static Sound* sounds = nullptr;
static int firstFreeVoice = 0;

void Audio_Init(void* platformData)
{
    sounds = (Sound*)malloc(sizeof(struct Sound) * MGDL_AUDIO_MAX_VOICES);
    for (int i = 0; i < MGDL_AUDIO_MAX_VOICES; i++)
    {
		Sound_InitEmpty(&sounds[i]);
    }

    Audio_Platform_Init(platformData);
	WavPlayer_Init();
	OggPlayer_Init();
	firstFreeVoice = 0;

}

void Audio_Deinit()
{
	Audio_Platform_Deinit();
	WavPlayer_Deinit();
	OggPlayer_Deinit();

    for (int i = 0; i < MGDL_AUDIO_MAX_VOICES; i++)
    {
		Sound_InitEmpty(&sounds[i]);
    }
	free(sounds);
	sounds = nullptr;
	firstFreeVoice = 0;
}

Sound* Audio_LoadSound(const char* filename, SoundFileType filetype)
{
	// Find first voice that is not in use
	int loadedIndex = firstFreeVoice;
	if (firstFreeVoice >= MGDL_AUDIO_MAX_VOICES)
	{
		Log_Error("Cannot load any more sounds, all voices in use");
		return nullptr;
	}
	Sound s;
	if (filetype == SoundWav)
	{
		s = WavPlayer_LoadSound(filename);
	}
	else if (filetype == SoundOgg)
	{
		s = OggPlayer_LoadSound(filename);
	}

	if (s.voiceNumber < 0)
	{
		// Failed to load
		Log_ErrorF("Failed to load sound from file %s\n", filename);
		return nullptr;
	}
	else {
		Log_InfoF("Loaded sound to voice %d\n", loadedIndex);
	}

	Sound_ToString(&s);
	s.type = filetype;
	sounds[loadedIndex] = s;
	firstFreeVoice += 1;

	return &sounds[loadedIndex];
}

void Audio_PlaySound(Sound* s)
{
	if (s == nullptr)
	{
		return;
	}
	if (s->type == SoundWav)
	{
		WavPlayer_PlaySound(s);
	}
	else if (s->type == SoundOgg)
	{
		OggPlayer_PlaySound(s);
	}
}

sizetype Audio_GetSoundSizeBytes(Sound* snd)
{
	switch (snd->type)
	{
	case SoundWav:
		return WavPlayer_GetSoundSizeBytes(snd);
		break;
	case SoundOgg:
		return OggPlayer_GetSoundSizeBytes(snd);
		break;
	}
	return 0;
}
u32 Audio_GetSoundElapsedMs(Sound* snd)
{
	switch (snd->type)
	{
	case SoundWav:
		return WavPlayer_GetSoundElapsedMs(snd);
		break;
	case SoundOgg:
		return OggPlayer_GetSoundElapsedMs(snd);
		break;
	}
	return 0;

}

mgdlAudioStateEnum Audio_GetSoundStatus(Sound* snd)
{
	switch (snd->type)
	{
	case SoundWav:
		return Audio_GetStaticBufferStatus(snd);
		break;
	case SoundOgg:
		return OggPlayer_GetSoundStatus(snd);
		break;
	}
	return Audio_StateInvalid;

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

