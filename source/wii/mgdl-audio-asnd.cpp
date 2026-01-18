#ifdef GEKKO
#include <mgdl/mgdl-audio.h>
#include <mgdl/mgdl-alloc.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/wii/mgdl-audio-asnd.h>
#include <asndlib.h>

// This is given by player as id to be given back to it in callback
static s32 streamingVoice = -1;
static SoundASND streamingSound;
#define ASND_STREAMING_BUFFERS 2
static void* streamingBuffers[ASND_STREAMING_BUFFERS];
static int activeStreamingBuffer = 0;

static SoundASND* soundDatas;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void Audio_Platform_Init(void* platformData)
{
	Log_Info("ASND Audio init\n");
	ASND_Init();
	soundDatas = (SoundASND*)valloc(sizeof(SoundASND) * MGDL_AUDIO_MAX_SOUNDS);
	for (int i = 0; i < MGDL_AUDIO_MAX_SOUNDS; i++)
	{
		soundDatas[i].buffer = nullptr;
		soundDatas[i].inUse = false;
		soundDatas[i].voiceNumber = SND_INVALID;
	}

	// Prepare streaming buffers
	for (int i = 0; i < 2; i++)
	{
		// The the buffers need to be aligned to 32
		streamingBuffers[i]= mgdl_AllocateAlignedMemory(MGDL_AUDIO_CALLBACK_BUFFER_SIZE);
	}

}
#pragma GCC diagnostic pop

void Audio_Platform_Deinit()
{
	ASND_Pause(1);
	for (int i = 0; i < MGDL_AUDIO_MAX_SOUNDS; i++)
	{
		if (soundDatas[i].buffer != nullptr)
		{
			free(soundDatas[i].buffer);
		}
		soundDatas[i].inUse = false;
		soundDatas[i].voiceNumber = SND_INVALID;
	}
	vfree(soundDatas);
	ASND_End();
}

static AudioCallbackFunction audioCallback = nullptr;
void Audio_Platform_SetCallback(AudioCallbackFunction callbackFunction)
{
	audioCallback = callbackFunction;
}

void Audio_Update(void)
{
	// ASND updates streaming automatically
	// Update streaming
}

static short mgdlFormatToASND(SoundSampleFormat format)
{
	switch (format)
	{
		case Format_Mono_8: return VOICE_MONO_8BIT; break;
		case Format_Mono_16: return VOICE_MONO_16BIT; break;
		case Format_Stereo_8: return VOICE_STEREO_8BIT; break;
		case Format_Stereo_16: return VOICE_STEREO_16BIT; break;
	}
	return SND_INVALID;
}

void* Audio_OpenStaticBuffer(Sound* inout_snd, sizetype byteAmount, u16 samplerate, SoundSampleFormat format)
{
	// Find first free sound data
	for (int i = 0; i < MGDL_AUDIO_MAX_SOUNDS; i++)
	{
		if (soundDatas[i].buffer == nullptr)
		{
			inout_snd->voiceNumber = i;

			// If all is valid, allocate memory for storing the wave data
			if (soundDatas[i].buffer != NULL)
			{ 
				// If already allocated, deallocate it for reallocation
				free(soundDatas[i].buffer);
			} 
			soundDatas[i].buffer = mgdl_AllocateAlignedMemory(byteAmount);
			memset(soundDatas[i].buffer, 0x00, byteAmount);

			// Store information needed for playback
			soundDatas[i].format = mgdlFormatToASND(format);
			soundDatas[i].freq = samplerate;
			soundDatas[i].sSize = byteAmount;
			soundDatas[i].channels = Sound_FormatToChannels(format);
			
			return soundDatas[i].buffer;
		}
	}
	return nullptr;
}


void Audio_CloseStaticBuffer(Sound* snd, void* buffer, sizetype bytesWritten)
{
	if (snd->voiceNumber < 0)
	{
		return;
	}

	// Swap stereo samples
	int sSize = bytesWritten;
	void* sData = buffer;
	u_short buff;
	if (soundDatas[snd->voiceNumber].format == Format_Stereo_8)
	{
		for (int s = 0; s < sSize; s += 2) {
			buff = ((u_char*)sData)[s];
			((u_char*)sData)[s] = ((u_char*)sData)[s + 1];
			((u_char*)sData)[s + 1] = buff;
		}
	}

	if (soundDatas[snd->voiceNumber].format == Format_Stereo_16)
	{
		for(int s = 0; s < sSize/2; s += 2) {
			buff = ((u_short*)sData)[s];
			((u_short*)sData)[s] = ((u_short*)sData)[s + 1];
			((u_short*)sData)[s + 1] = buff;
		}
	}

	DCFlushRange(sData, sSize);
}

sizetype Audio_GetStaticBufferSize(Sound* snd)
{
	return soundDatas[snd->voiceNumber].sSize;
}

mgdlAudioStateEnum Audio_GetStaticBufferStatus(Sound* snd)
{
	SoundASND* sound = &soundDatas[snd->voiceNumber];
	s32 status = ASND_StatusVoice(sound->voiceNumber);
	if (status == SND_WORKING)
	{
		return Audio_StatePlaying;
	}
	else if (status == SND_UNUSED)
	{
		return Audio_StateStopped;
	}
	else
	{
		return Audio_StatePaused;
	}
}

void Audio_PlayStaticBuffer(Sound* snd)
{
	SoundASND* sound = &soundDatas[snd->voiceNumber];
	// Simple play function
	if (sound->buffer == nullptr)
		return;

	sound->voiceNumber = ASND_GetFirstUnusedVoice();
	if (sound->voiceNumber == SND_INVALID)
	{
		return;
	}

	// TODO master volume etc.
	s32 pitch = 1;
	s32 volume = 255;

	ASND_SetVoice(
		sound->voiceNumber,
		sound->format,
		sound->freq*pitch,
		0,
		sound->buffer,
		sound->sSize,
		volume,
		volume,
		NULL);

	ASND_Pause(0);
}

void Audio_StopStaticBuffer(Sound* snd)
{
	SoundASND* sound = &soundDatas[snd->voiceNumber];
	if (sound->voiceNumber != SND_INVALID)
	{
		ASND_StopVoice(sound->voiceNumber);
	}
}

void Audio_PauseStaticBuffer(Sound* snd, bool paused)
{
	SoundASND* sound = &soundDatas[snd->voiceNumber];
	if (sound->voiceNumber != SND_INVALID)
	{
		ASND_PauseVoice(sound->voiceNumber, paused ? 1 : 0);
	}
}

u32 Audio_GetStaticBufferElapsedMs(Sound* snd)
{
	SoundASND* sound = &soundDatas[snd->voiceNumber];
	if (sound->voiceNumber != SND_INVALID)
	{
		return ASND_GetTimerVoice(sound->voiceNumber);
	}
	return 0;
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void Audio_SetStaticBufferElapsedMs(Sound* snd, u32 milliseconds)
{
	// Not possible on Wii
}
#pragma GCC diagnostic pop

// ASND calls this when the first buffer is empty
void ASND_Callback(s32 asndVoice)
{
	if (asndVoice != 0)
	{
		return;
	}
	// Switch to next buffer
	activeStreamingBuffer = (activeStreamingBuffer + 1)%ASND_STREAMING_BUFFERS;
	//Log_InfoF("Active streaming buffer %d\n", activeStreamingBuffer);

	u32 bytesWritten = 0;
	// Ask the player for more audio with the voice number
	// provided in StartStream
	audioCallback(streamingVoice, streamingBuffers[activeStreamingBuffer], streamingSound.sSize, &bytesWritten);

	// Make sure memory is flushed
	DCFlushRange(streamingBuffers[activeStreamingBuffer], MGDL_AUDIO_CALLBACK_BUFFER_SIZE);

	// Send the audio to ASND using the streaming Voice number 0
	ASND_AddVoice(streamingSound.voiceNumber, streamingBuffers[activeStreamingBuffer], bytesWritten);
}

void Audio_Platform_StartStream(Sound* s, s32 sampleRate, SoundSampleFormat format)
{
	// Note Voice Number 0 is reserved for streaming
	streamingSound.voiceNumber = 0;

	// Link to Sound*
	streamingVoice = s->voiceNumber;

	// Prepare first buffer

		// Store information needed for playback
	streamingSound.format = mgdlFormatToASND(format);
	streamingSound.freq = sampleRate;
	streamingSound.sSize = MGDL_AUDIO_CALLBACK_BUFFER_SIZE;
	streamingSound.channels = Sound_FormatToChannels(format);

	// Fill both buffers with silence
	memset(streamingBuffers[0], 0, MGDL_AUDIO_CALLBACK_BUFFER_SIZE);
	memset(streamingBuffers[1], 0, MGDL_AUDIO_CALLBACK_BUFFER_SIZE);

	// Fill the first buffer
	u32 bytesWritten = 0;
	audioCallback(streamingVoice, streamingBuffers[activeStreamingBuffer], streamingSound.sSize, &bytesWritten);

	// Flush to memory
	DCFlushRange(streamingBuffers[activeStreamingBuffer], streamingSound.sSize);

	// TODO master volume etc.
	s32 pitch = 1;
	s32 volume = 255;

	// Send first buffer
	ASND_SetVoice(
		streamingSound.voiceNumber,
		streamingSound.format,
		streamingSound.freq*pitch,
		0,
		streamingBuffers[activeStreamingBuffer],
		streamingSound.sSize,
		volume,
		volume,
		ASND_Callback);

	ASND_Pause(0);
}
void Audio_Platform_StopStream(Sound* s)
{
	if (streamingVoice == s->voiceNumber)
	{
		ASND_StopVoice(streamingSound.voiceNumber);
	}

}
void Audio_Platform_PauseStream(Sound* s)
{
	if (streamingVoice == s->voiceNumber)
	{
		ASND_PauseVoice(streamingSound.voiceNumber, 1);
	}
}
void Audio_Platform_ResumeStream(Sound* s)
{
	if (streamingVoice == s->voiceNumber)
	{
		ASND_PauseVoice(streamingSound.voiceNumber, 0);
	}
}
#endif
