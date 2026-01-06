#ifdef GEKKO
#include <mgdl/wii/mgdl-audio-asnd.h>


static SoundASND* soundDatas;
void Audio_Platform_Init()
{
	soundDatas = (SoundASND*)malloc(sizeof(SoundASND) * MGDL_AUDIO_MAX_VOICES);
	for (int i = 0; i < MGDL_AUDIO_MAX_VOICES; i++)
	{
		soundDatas[i].buffer = nullptr;
		soundDatas[i].inUse = false;
		soundDatas[i].voiceNumber = SND_INVALID;
	}
}

void Audio_Platform_Deinit()
{

}

static short mgdlFormatToASND(SoundSampleFormat format)
{
	switch (format)
	{
		Sound_Mono_s8: return VOICE_MONO_8BIT_U; break;
		Sound_Mono_s16: return VOICE_MONO_16BIT_LE; break;
		Sound_Stereo_s8: return VOICE_STEREO_8BIT_U; break;
		Sound_Stereo_s1: return VOICE_STEREO_16BIT_LE; break;
	}
	return SND_INVALID;
}

void* Audio_Platform_OpenStaticBuffer(Sound* inout_snd, sizetype byteAmount, u16 samplerate, SoundSampleFormat format)
{
	// Find first free sound data
	for (int i = 0; i < MGDL_AUDIO_MAX_VOICES; i++)
	{
		if (soundDatas[i].buffer == nullptr)
		{
			inout_snd->voiceNumber = i;

			// If all is valid, allocate memory for storing the wave data
			if (soundDatas[i]->buffer != NULL) 
			{ 
				// If already allocated, deallocate it for reallocation
				vfree(soundDatas[i]->buffer); 
			} 
			soundDatas[i]->buffer = valloc(byteAmount);
			memset(soundDatas[i]->buffer, 0x00, byteAmount);

			// Store information needed for playback
			soundDatas[i]->format = mgdlFormatToASND(format);
			soundDatas[i]->freq = samplerate;
			soundDatas[i]->sSize = byteAmount;
			soundDatas[i]->channels = Sound_FormatToChannels(format);
			
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
	if (snd->format == Sound_Stereo_s8)
	{
		for (sizetype s = 0; s < sSize; s += 2) {	
			buff = ((u_char*)sData)[s];
			((u_char*)sData)[s] = ((u_char*)sData)[s + 1];
			((u_char*)sData)[s + 1] = buff;
		}
	}
	else if (snd->format == Sound_Stereo_s16)
	{
		for(sizetype s = 0; s < sSize/2; s += 2) {
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
#endif
