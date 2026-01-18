#include <mgdl/mgdl-sound.h>
#include <mgdl/mgdl-logger.h>

Sound* Sound_Create(void)
{
    Sound* sound = (Sound*)malloc(sizeof(Sound));
    Sound_InitEmpty(sound);
    return sound;
}
void Sound_InitEmpty(Sound* sound)
{
    sound->voiceNumber = -1;
    sound->isLooping = false;
    sound->type = SoundWav;
}
void Sound_Init(Sound* snd, s32 voiceNumber, SoundFileType filetype)
{
    Sound_InitEmpty(snd);
    snd->voiceNumber = voiceNumber;
    snd->type = filetype;
}

int Sound_FormatToChannels(SoundSampleFormat format)
{
    switch (format)
    {
    case Format_Mono_16:
        return 1;
    case Format_Mono_8:
        return 1;
    case Format_Stereo_16:
        return 2;
    case Format_Stereo_8:
        return 2;
    }
    return 0;
}
bool Sound_GetLooping(Sound* sound) { return sound->isLooping; }

static const char* soundType(SoundFileType ft)
{
    if (ft == SoundOgg) { return "Ogg"; }
    else if (ft == SoundWav) { return "Wav"; }
    else if (ft == SoundMp3) { return "Mp3"; }
    else { return "Unknown";}
}

void Sound_ToString(Sound* sound)
{
    if (sound != nullptr)
    {
        Log_InfoF("Sound: voice: %d, type %s\n", sound->voiceNumber, soundType(sound->type));
    }

}
