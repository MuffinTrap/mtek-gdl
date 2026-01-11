#if defined(MGLD_PLATFORM_LINUX) || defined(MGDL_PLATFORM_MAC)

#include <mgdl/pc/mgdl-sound-openal.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-alloc.h>
#include <mgdl/mgdl-openal.h>
#include <mgdl/mgdl-logger.h>

#include <sndfile.h>

// OpenAL sound
static ALCdevice* device;
static ALCcontext* context;
static SoundOpenAL* soundDatas


/* Streaming audio */
static AudioCallbackFunction audioCallback = nullptr;
void Audio_Platform_SetCallback(AudioCallbackFunction callbackFunction)
{
	audioCallback = callbackFunction;
}

// one buffer is playing, one is ready to go and one is filled
#define MGDL_NUM_STREAMING_BUFFERS 3
static ALuint streamingSource;
static StreamBuffer streamingBuffers[MGDL_NUM_STREAMING_BUFFERS];
static ALenum streamingFormat;
static ALsizei streamingFreq;
static u32 streamingVoiceNumber;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void Audio_Platform_Init(void* platformData)
{
    Log_Info("Setting up OpenAL Audio Device.\n");
    // Initialize OpenAL
    device = alcOpenDevice(NULL);
    if (!device) {
        Log_Error("Failed to open OpenAL device\n");
        return;
    }
    Log_Info("Setting up OpenAL Audio Contex.\n");
    context = alcCreateContext(device, NULL);
    if (alcGetError(device) != ALC_NO_ERROR || !context) {
        Log_Error("Failed to create OpenAL context\n");
        alcCloseDevice(device);
        return;
    }

    ALboolean contextMadeOK = alcMakeContextCurrent(context);
    if (contextMadeOK != AL_TRUE)
    {
        Log_Error("Failed to make OpenAL context current\n");
        alcCloseDevice(device);
        return;
    }

    Log_Info("OpenAL context created\n");
	soundDatas = (SoundOpenAL*)malloc(sizeof(SoundOpenAL) * MGDL_AUDIO_MAX_SOUNDS);
	for (int i = 0; i < MGDL_AUDIO_MAX_SOUNDS; i++)
	{
        soundDatas[i].inUse = false;
    }
	for (int i = 0; i < MGDL_NUM_STREAMING_BUFFERS; i++)
	{
        streamingBuffers[i].tempBuffer = (s16*)malloc(MGDL_AUDIO_CALLBACK_BUFFER_SIZE);
        alGenBuffers(1, &streamingBuffers[i].bufferName);
    }
    alGenSources(1, &streamingSource);
}
#pragma GCC diagnostic pop

void Audio_Platform_Deinit()
{
	for (int i = 0; i < MGDL_AUDIO_MAX_SOUNDS; i++)
	{
        if (soundDatas[i].inUse)
        {
            SoundOpenAL* sound = soundDatas[s->voiceNumber];

            alDeleteSources(1, &sound->source);
            alDeleteBuffers(1, &sound->buffer);
            sound->inUse = false;
        }
    }
    free(soundDatas);
	for (int i = 0; i < MGDL_NUM_STREAMING_BUFFERS; i++)
	{
        free(streamingSamples[i]);
    }
    free(streamingSamples);

	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

ALenum mgdlFormatToOpenAL(SoundSampleFormat format)
{
    switch(format)
    {
        case Audio_Mono_8: return AL_FORMAT_MONO8;
        case Audio_Mono_16 return AL_FORMAT_MONO16;
        case Audio_Stereo_8 return AL_FORMAT_STEREO16;
        case Audio_Stereo_16 return AL_FORMAT_STEREO16;
    }
    return 0;
}


void* Audio_OpenStaticBuffer(Sound* inout_snd, sizetype byteCount, u16 samplerate, SoundSampleFormat format)
{
	s32 voiceNumber = -1;
	for (int i = 0; i < MGDL_AUDIO_MAX_SOUNDS; i++)
	{
		if (soundDatas[i].inUse == false)
		{
			voiceNumber = i;
			break;
		}
	}
	if (voiceNumber == -1)
	{
		OutputDebugStringA("No more voice slots free\n");
		Log_Error("No more voice slots free\n");
        inout_snd->voiceNumber = voiceNumber;
		return nullptr;
	}
	SoundOpenAL* snd = &soundDatas[voiceNumber];

    alCall(alGenBuffers,1, &snd->buffer);
    alCall(alGenSources,1, &snd->source);
    snd->format = mgdlFormatToOpenAL(format);
    snd->sampleRate = samplerate;

    inout_snd->voiceNumber = voiceNumber;

    snd->tempBuffer = (ALvoid*)malloc(byteCount);
    return snd->tempBuffer;
}

void Audio_CloseStaticBuffer(Sound* snd, void* buffer, sizetype bytesWritten)
{
    // Fill OpenAL buffer with audio data
    SoundOpenAL* soundAL = &soundDatas[snd->voiceNumber];
    alCall(alBufferData, soundAL->buffer, // Buffer id
           soundAL->format,
           soundAL->tempBuffer, // data pointer
           bytesWritten,
           soundAL->samplerate);

    soundAL->sizeBytes = bytesWritten;

    // Set the source's buffer
    alCall(alSourcei, soundAL->source, AL_BUFFER, soundAL->buffer);
    free(soundAL.tempBuffer);
}

void Audio_PlayStaticBuffer(Sound* snd)
{
    alSourcePlay(soundDatas[snd->voiceNumber].source)
}
void Audio_StopStaticBuffer(Sound* snd)
{
    alSourceStop(soundDatas[snd->voiceNumber].source)
}
sizetype Audio_GetStaticBufferSize(Sound* snd)
{
	return soundDatas[snd->voiceNumber].sizeBytes;
}

u32 Audio_GetStaticBufferElapsedMs(Sound* snd)
{
	ALfloat secOffset;
	alGetSourcef(soundDatas[snd->voiceNumber].source, AL_SEC_OFFSET, &secOffset);
	return (u32)(secOffset * 1000);
}

void Audio_SetBufferElapsed(Sound* snd, u32 milliseconds)
{
	alSourcef(soundDatas[snd->voiceNumber].source, AL_SEC_OFFSET, (float)milliseconds/1000.0f);

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

void Sound_SetLooping(Sound* sound, bool looping)
{
    sound->isLooping = looping;
    alSourcei(sound->source, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
}

static mgdlAudioStateEnum GetOpenALSourceStatus(ALuint source)
{
    ALint sourceState;
    alGetSourcei(source, AL_SOURCE_STATE, &sourceState);

    if (sourceState == AL_PLAYING) {
        return Audio_StatePlaying;
    } else if (sourceState == AL_PAUSED) {
        return Audio_StatePaused;
    } else if (sourceState == AL_STOPPED) {
        return Audio_StateStopped;
    } else if (sourceState == AL_INITIAL) {
        return Audio_StateStopped;
    }
    return Audio_StateInvalid;

}

mgdlAudioStateEnum Audio_GetStaticBufferStatus(Sound* sound) {
	// Get the play state of the audio source
    return GetOpenALSourceStatus(soundDatas[snd->voiceNumber].source);
}

static void FillAndQueue(StreamBuffer* buffer)
{
    u32 bytesWritten = 0;
    audioCallback(streamingVoiceNumber, buffer->tempBuffer, MGDL_AUDIO_CALLBACK_BUFFER_SIZE, &bytesWritten);

    // Copy data to OpenAL buffer
    alBufferData(buffer->bufferName, streamingFormat,
    (ALvoid*)buffer->tempBuffer, bytesWritten, streamingFreq);

    // Queue buffer
    alCall(alSourceQueueBuffers, streamingSource, 1, &buffer->bufferName);
}

void Audio_Platform_StartStream(Sound* snd, s32 sampleRate)
{
    streamingVoiceNumber = snd->voiceNumber;
    streamingFreq = sampleRate;
    // Fill all buffers
    for (ALsizei i = 0; i < MGDL_NUM_STREAMING_BUFFERS; i++)
    {
        FillAndQueue(&streamingBuffers[i]);
    }

    alCall(alSourceQueueBuffers, streamingSource, MGDL_NUM_STREAMING_BUFFERS, streamingBuffers);

    alSourcePlay(streamingSource);
}

void Audio_Platform_StopStream(Sound* snd)
{
    if (snd->voiceNumber == streamingVoiceNumber)
    {
        alSourceStop(streamingSource);
    }
}

void Audio_Update()
{
    if (GetOpenALSourceStatus(streamingSource) != Audio_StatePlaying)
    {
        return;
    }
    ALsizei processedAmount = 0;
    ALuint processedNames[MGDL_NUM_STREAMING_BUFFERS];

    // Check if any of the buffers have been processed
    alCall(alSourcei, streamingSource, AL_BUFFERS_PROCESSED, &processedAmount);
    if (processedAmount > 0)
    {
        // Detach all processed buffers
        alCall(alSourceUnqueueBuffers, streamingSource, processedAmount, processedNames);

        // Fill each processed buffer with new data
        u32 bytesWritten = 0;
        for (ALsizei i = 0; i < processedAmount; i++)
        {
            // Find matching StreamBuffer for each processed buffer name
            for (int p = 0; p < processedAmount; p++)
            {
                for (int i = 0; i < MGDL_NUM_STREAMING_BUFFERS; i++)
                {
                    bool found = false;
                    if (streamingBuffers[i].bufferName == processedNames[p])
                    {
                        // Get data from player
                        FillAndQueue(&streamingBuffers[i]);

                        found = true;
                        break;
                    }
                }
                if(found)
                {
                    break;
                }
            }
        }
    }
}

#endif
