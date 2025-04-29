#include <mgdl.h>

#ifdef GEKKO
#include <oggplayer.h>
#endif

static float MasterSfxVolume = 0;
static float MasterMusicVolume = 0;
static float UserMusicVolume = 0;

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
