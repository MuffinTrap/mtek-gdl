#include <mgdl/mgdl-audio.h>
#include <mgdl/mgdl-sound.h>
#include <mgdl/mgdl-logger.h>


static float MasterSfxVolume = 0;
static float MasterMusicVolume = 0;
static float UserMusicVolume = 0;

static Sound* sounds = nullptr;

void Audio_Init(void* platformData)
{
    Audio_Platform_Init(platformData);
    sounds = (Sound*)malloc(sizeof(struct Sound) * MGDL_AUDIO_MAX_VOICES);
    for (int i = 0; i < MGDL_AUDIO_MAX_VOICES; i++)
    {
        sounds[i].sizeBytes = 0;
        sounds[i].voiceNumber = -1;
    }
}

Sound* Audio_LoadSound(const char* filename)
{
    if (filename != "")
    {
        for (int i = 0; i < MGDL_AUDIO_MAX_VOICES; i++)
        {
            if (sounds[i].sizeBytes <= 0)
            {
                Sound s = Audio_Platform_LoadSound(filename, i);
                if (s.sizeBytes > 0)
                {
                    sounds[i] = s;
					return &sounds[i];
                }
                else
                {
                    // Failed to load
                    Log_ErrorF("Failed to load sound from file %s", filename);
                    break;
                }
            }
        }
    }
    return nullptr;
}

void Audio_PlaySound(Sound* s)
{
    if (s != nullptr && s->voiceNumber >= 1); // 0 is music voice
    {
        Audio_Platform_PlaySound(s->voiceNumber);
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
