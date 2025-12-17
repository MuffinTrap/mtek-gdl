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
    sound->voiceNumber = -1;
    sound->isLooping = false;
    sound->secondsOffset = 0.0f;
}

bool Sound_GetLooping(Sound* sound) { return sound->isLooping; }
