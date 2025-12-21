#include <mgdl/mgdl-sound.h>

Sound* Sound_Create(void)
{
    Sound* sound = (Sound*)malloc(sizeof(Sound));
    Sound_InitEmpty(sound);
    return sound;
}
void Sound_InitEmpty(Sound* sound)
{
    sound->sizeBytes = 0;
    sound->channels = 0;
    sound->voiceNumber = -1;
    sound->isLooping = false;
    sound->elapsedSeconds = 0.0f;
    sound->type = SoundWav;
}
void Sound_Init(Sound* snd, s32 voiceNumber, s32 channels, sizetype sizeBytes, SoundFileType filetype)
{
    Sound_InitEmpty(snd);
    snd->voiceNumber = voiceNumber;
    snd->channels = channels;
    snd->sizeBytes = sizeBytes;
    snd->type = filetype;
}

bool Sound_GetLooping(Sound* sound) { return sound->isLooping; }
float Sound_GetElapsedSeconds(Sound* sound) { return sound->elapsedSeconds; }
