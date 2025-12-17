#pragma once

#if defined(MGLD_PLATFORM_LINUX) || defined(MGDL_PLATFORM_MAC)

#include <mgdl/pc/mgdl-sound-openal.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-openal.h>
#include <mgdl/mgdl-logger.h>

#include <sndfile.h>

static SoundOpenAL* soundDatas

void Audio_Platform_Init(void* platformData)
{
	soundDatas = (SoundOpenAL*)malloc(sizeof(SoundOpenAL) * MGDL_AUDIO_MAX_VOICES);
}

Sound Audio_Platform_LoadSound(const char* filename, s32 voiceNumber)
{
    Sound s;
    Sound_InitEmpty(&s);
    Log_InfoF("Loading sound from %s\n", filename);

	// Open the WAV file
    SF_INFO sfinfo;
    SNDFILE* sndfile = sf_open(filename, SFM_READ, &sfinfo);
    if (!sndfile) {
        mgdl_assert_printf(false, "Sound_Load did not find %s\n", filename);
        return s;
    }

    ALuint buffer, source;

    alCall(alGenBuffers,1, &buffer);
    alCall(alGenSources,1, &source);

    // Read audio data from the WAV file
    ALsizei dataSize = sfinfo.frames * sfinfo.channels * sizeof(s16);
    ALuint format = AL_FORMAT_STEREO16;
    if (sfinfo.channels == 1)
    {
        format = AL_FORMAT_MONO16;
    }
    ALvoid* data = (ALvoid*)malloc(dataSize);
    mgdl_assert_print(data != nullptr, "Out of memory!");
    if (data == nullptr)
    {
        return s;
    }

    SoundOpenAL* sound = soundDatas[voiceNumber];
    sound->sndfile = sndfile;
    sound->buffer = buffer;
    sound->sSize = dataSize;
    sound->source = source;

    s.voiceNumber = voiceNumber;
    s.sizeBytes = dataSize;

    sf_read_raw(sound->sndfile, data, dataSize);

    // Fill OpenAL buffer with audio data
    alCall(alBufferData, sound->buffer, format, data, dataSize, sfinfo.samplerate);

    // Set the source's buffer
    alCall(alSourcei, sound->source, AL_BUFFER, sound->buffer);

    free(data);
    sf_close(sndfile);
    Log_Info("Sound loaded\n");

    return s;
}

void Audio_Platform_UnloadSound(Sound* s) 
{
    SoundOpenAL* sound = soundDatas[s->voiceNumber];

    alDeleteSources(1, &sound->source);
    alDeleteBuffers(1, &sound->buffer);
}

void Sound_Play(Sound* sound) {
	alSourcePlay(sound->source);
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void Sound_PlayEx(Sound* sound, float pitchOffset, float volumePercent) {
	alSourcePlay(sound->source);
}
#pragma GCC diagnostic pop

void Sound_Stop(Sound* sound) {
	alSourceStop(sound->source);
}

void Sound_SetPaused(Sound* sound, bool pause) {
	if (pause)
    {
    	alSourcePause(sound->source);
    }
	else
    {
    	alSourcePlay(sound->source);
    }
}

void Sound_SetElapsedSeconds(Sound* sound, float elapsed)
{
	alSourcef(sound->source, AL_SEC_OFFSET, elapsed);
}


void Sound_SetLooping(Sound* sound, bool looping)
{
    sound->isLooping = looping;
    alSourcei(sound->source, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
}




float Sound_GetElapsedSeconds(Sound* sound) {
	ALfloat secOffset;
	alGetSourcef(sound->source, AL_SEC_OFFSET, &secOffset);
	return secOffset;
}

SoundStatus Sound_GetStatus(Sound* sound) {
	// Get the play state of the audio source
    ALint sourceState;
    alGetSourcei(sound->source, AL_SOURCE_STATE, &sourceState);

    if (sourceState == AL_PLAYING) {
        return SoundStatus::SoundPlaying;
    } else if (sourceState == AL_PAUSED) {
        return SoundStatus::SoundPaused;
    } else if (sourceState == AL_STOPPED) {
        return SoundStatus::SoundStopped;
    } else if (sourceState == AL_INITIAL) {
        return SoundStatus::SoundInitial;
    }
    return SoundStatus::SoundInitial;
}

#endif
