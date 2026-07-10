#include <mgdl/mgdl-sound.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-memory.h>
#include <mgdl/mgdl-audio.h>
#include <mgdl/mgdl-util.h>

Sound* Sound_Create(void)
{
    Sound* sound = (Sound*)mgdl_AllocateGraphicsMemory(sizeof(Sound));
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

SoundBank* SoundBank_Create(u8 soundAmount)
{
    SoundBank* bank = (SoundBank*)mgdl_AllocateGraphicsMemory(sizeof(SoundBank));
    bank->sounds = (Sound**)mgdl_AllocateGraphicsMemory(sizeof(Sound*) * soundAmount);
    for(int i = 0; i < soundAmount; i++)
    {
        bank->sounds[i] = nullptr;
    }
    bank->playmode = SoundBank_Random;
    bank->currentIndex = 0;
    return bank;
}

SoundBank* SoundBank_Destroy(SoundBank* bank)
{
   mgdl_FreeGraphicsMemory(bank->sounds);
   mgdl_FreeGraphicsMemory(bank);
   return nullptr;
}
void SoundBank_SetSound(SoundBank* bank, u8 index, Sound* sound)
{
    if (index < bank->soundAmount)
    {
        bank->sounds[index] = sound;
    }
}
void SoundBank_SetMode(SoundBank* bank, mgdlSoundBankPlayMode mode)
{
    bank->playmode = mode;

}
void SoundBank_PlayNext(SoundBank* bank)
{
    u8 nextIndex = 0;
    switch(bank->playmode)
    {
        case SoundBank_Random:
        {
            nextIndex = Random_Int(0, bank->soundAmount-1);
        }
        break;
        case SoundBank_RunThrough:
        {
            nextIndex = bank->currentIndex + 1;
            nextIndex = nextIndex % bank->soundAmount;
        }
        break;
        case SoundBank_Shuffle:
        {
            if (bank->soundAmount > 2)
            {
                u8 test = bank->currentIndex;
                while(test == bank->currentIndex)
                {
                    test = Random_Int(0, bank->soundAmount-1);
                }
                nextIndex = test;
            }
            else
            {
                nextIndex = bank->currentIndex + 1;
                nextIndex = nextIndex % bank->soundAmount;
            }
        }
        break;
    }

    if (bank->sounds[nextIndex] != nullptr)
    {
        Sound_Play(bank->sounds[nextIndex]);
        bank->currentIndex = nextIndex;
    }
}

void SoundBank_Stop(SoundBank* bank)
{
    Sound* playing = bank->sounds[bank->currentIndex];
    Sound_Stop(playing);
}

bool SoundBank_IsPlaying(SoundBank* bank)
{
    Sound* playing = bank->sounds[bank->currentIndex];
    return Sound_GetStatus(playing) == Audio_StatePlaying;
}
