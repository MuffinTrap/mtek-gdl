#include <mgdl/mgdl-audio.h>
#if defined(MGDL_WINDOWS_NATIVE)

#include <mgdl/pc/mgdl-sound-directsound.h>

#include <sndfile.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-util.h>
#include <Dsound.h>


// Pointer to Object created by DirectSoundCreate
static LPDIRECTSOUND DirectSoundObject = nullptr;
// Pointer to Buffer created by direct sound
static LPDIRECTSOUNDBUFFER DirectSoundPrimaryBuffer = nullptr;

// Main buffer format
static WAVEFORMATEX waveFormatPrimary = { 0 };

// One streaming buffer for music
AudioCallbackFunction audioCallback = nullptr;
static LPDIRECTSOUNDBUFFER streamingBuffer = nullptr;
static DWORD streamingBufferSize = 0;
static DWORD lastWriteCursorPosition = -MGDL_AUDIO_CALLBACK_BUFFER_SIZE;
static DWORD lastPlayCursorPosition = 0;
static DWORD lastWritePosition = 0; // Make sure to always continue writing from the last point
static void WriteToStream(DWORD bytesToWrite);

// Multiple buffers for music and sound effects
static SoundDirectSound* soundDatas;
static bool Create_Buffer(u32 sizeBytes, WORD channels, LPDIRECTSOUNDBUFFER* bufferPtr);

// static HANDLE soundBufferEmptyEvent;

#define DIRECT_SOUND_CREATE(name) HRESULT WINAPI name(LPGUID deviceGUID, LPDIRECTSOUND* objectAddress, LPUNKNOWN aggregate)

typedef DIRECT_SOUND_CREATE(dsound_create);

DIRECT_SOUND_CREATE(DirectSoundCreateStub)
{
	return DSERR_NODRIVER;
}

static HRESULT(*DirectSound_Create_FuncP)(LPGUID deviceGUID, LPDIRECTSOUND* objectAddress, LPUNKNOWN aggregate) = DirectSoundCreateStub;


// Playback and audio state

static s32 activeStreamingSound = -1;

static DWORD ringBufferWritePoint;

void Audio_Platform_SetCallback(AudioCallbackFunction callbackFunction)
{
	audioCallback = callbackFunction;
}

void Audio_Platform_Init(void* platformData)
{
	HWND windowHandle = *(HWND*)platformData;
	OutputDebugStringA("Initializing DirectSound\n");

	soundDatas = (SoundDirectSound*)malloc(sizeof(SoundDirectSound) * MGDL_AUDIO_MAX_VOICES);
	for (int i = 0; i < MGDL_AUDIO_MAX_VOICES; i++)
	{
		soundDatas[i].channels = 0;
		soundDatas[i].buffer = nullptr;
		soundDatas[i].inUse = false;
	}

	// Init windows Multimedia audio system
	// Load the library for DirectSound audio
	HMODULE DirectSoundLibrary = LoadLibraryA("dsound.dll");;
	if (DirectSoundLibrary == NULL)
	{
		Log_Error("Failed load dsound.dll\n");
		OutputDebugStringA("Failed load dsound.dll\n");
		return;
	}
	DirectSound_Create_FuncP = (dsound_create*)GetProcAddress(DirectSoundLibrary, "DirectSoundCreate");

	if (DirectSound_Create_FuncP)
	{
		// Get DirectSound object
		HRESULT DCreateResult = DirectSound_Create_FuncP(NULL, &DirectSoundObject, NULL);
		if (DCreateResult != DS_OK)
		{
			// Failed to create Direct Sound object
			Log_Error("Failed to create direct sound object");
			OutputDebugStringA("Failed to create direct sound object");

			return;
		}
		OutputDebugStringA("Created direct sound object\n");
		// Initialize Direct sound object

		HRESULT cooperativeResult = DirectSoundObject->SetCooperativeLevel(windowHandle, DSSCL_PRIORITY);
		if (cooperativeResult != DS_OK)
		{
			// Failed to set cooperative level
			Log_Error("Failed to set cooperative level");
			OutputDebugStringA("Failed to set cooperative level");

			return;
		}


		// Creates primary hardware buffer
		// for DirectSound.
		DSBUFFERDESC primaryBufferDescription = { 0 };
		primaryBufferDescription.dwSize = sizeof(DSBUFFERDESC); // Size of the structure
		primaryBufferDescription.dwFlags = DSBCAPS_PRIMARYBUFFER; // Capabilities
		primaryBufferDescription.dwBufferBytes = 0; // Size of buffer, 0 for primary
		primaryBufferDescription.dwReserved = 0; // Must be 0
		primaryBufferDescription.lpwfxFormat = NULL; // Format, NULL for primary
		primaryBufferDescription.guid3DAlgorithm = DS3DALG_DEFAULT; // Virtualization algo

		// Create a primary buffer
		HRESULT createBufferResult = DirectSoundObject->CreateSoundBuffer(&primaryBufferDescription, &DirectSoundPrimaryBuffer, NULL);
		if (createBufferResult != DS_OK)
		{
			// Failed to create buffer
			Log_Error("Failed to create primary buffer\n");
			OutputDebugStringA("Failed to create primary buffer\n");

			return;
		}
		waveFormatPrimary = { 0 };
		waveFormatPrimary.wFormatTag = WAVE_FORMAT_PCM;
		waveFormatPrimary.nChannels = 2;
		waveFormatPrimary.nSamplesPerSec = MGDL_AUDIO_SAMPLE_RATE;
		waveFormatPrimary.wBitsPerSample = 16; // 16 bit audio
		waveFormatPrimary.nBlockAlign = (waveFormatPrimary.nChannels * waveFormatPrimary.wBitsPerSample) / 8; // 8 is bits per byte
		waveFormatPrimary.nAvgBytesPerSec = waveFormatPrimary.nSamplesPerSec * waveFormatPrimary.nBlockAlign;
		waveFormatPrimary.cbSize = 0; // Ignored in PCM

		HRESULT setPrimaryFormatResult = DirectSoundPrimaryBuffer->SetFormat(&waveFormatPrimary);
		if (setPrimaryFormatResult != DS_OK)
		{
			// Failed to set buffer format
			OutputDebugStringA("Failed to set primary buffer format");

			Log_Error("Failed to set primary buffer format");
		}

		// Create streaming buffer for voice number 0 that is the music.
		OutputDebugStringA("Creating streaming buffer\n");
		streamingBufferSize = waveFormatPrimary.nAvgBytesPerSec * 2;
		Create_Buffer(streamingBufferSize, 2, &streamingBuffer);

		// Set notification callback that DirectSound calls
		// when buffer needs to be filled
	// Create the callback event, but how to use it?
		// NOTE does not work on looping buffers :I
		/*
		soundBufferEmptyEvent = CreateEventA(NULL, FALSE, FALSE, "mgdl_DSBufferEmpty");
		LPDIRECTSOUNDNOTIFY soundNotify;
		DSBPOSITIONNOTIFY positionNotify;
		// is the notify supported
		HRESULT notifySetResult = DirectSoundWriteBuffer->QueryInterface(IID_IDirectSoundNotify, (LPVOID*)&soundNotify);
		if (notifySetResult == DS_OK)
		{
			positionNotify.dwOffset = DSBPN_OFFSETSTOP;
			positionNotify.hEventNotify = soundBufferEmptyEvent;
			notifySetResult = soundNotify->SetNotificationPositions(1, &positionNotify);

		}

		// Test the event to see if more samples are needed
		HRESULT waitResult = WaitForSingleObject(soundBufferEmptyEvent, 0);
		if (waitResult == WAIT_OBJECT_0)
		{

		}
		*/
	}
}


void* Audio_OpenStaticBuffer(Sound* inout_snd, sizetype byteCount, u16 samplerate, SoundSampleFormat format)
{
	// Find first free sound
	s32 voiceNumber = -1;
	for (int i = 0; i < MGDL_AUDIO_MAX_VOICES; i++)
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
		return nullptr;
	}

	u32 sizeBytes = byteCount;
	short channels = Sound_FormatToChannels(format);
	LPDIRECTSOUNDBUFFER* bufferPtr = &soundDatas[voiceNumber].buffer;
	if (Create_Buffer(sizeBytes, channels, bufferPtr))
	{
		// Lock, Fill and Unlock buffer
		LPVOID lpWrite;
		DWORD dwLength;
		if (DS_OK == (*bufferPtr)->Lock(
			0,
			0,
			&lpWrite,
			&dwLength,
			NULL,
			NULL,
			DSBLOCK_ENTIREBUFFER))
		{
			soundDatas[voiceNumber].buffer->Unlock(lpWrite, dwLength, 0, 0);
			soundDatas[voiceNumber].channels = channels;
			soundDatas[voiceNumber].inUse = true;
			soundDatas[voiceNumber].sizeBytes = sizeBytes;
			soundDatas[voiceNumber].elapsedSeconds = 0.0f;

			// Set common values and return
			inout_snd->voiceNumber = voiceNumber;

			return lpWrite;
		}
		else
		{
			OutputDebugStringA("Failed to lock buffer\n");
		}

		// TODO : if fails, try to Restore buffer
	}
	else
	{
		OutputDebugStringA("Failed to create buffer\n");
	}
	return nullptr;

}

void Audio_CloseStaticBuffer(Sound* snd, void* buffer, sizetype bytesWritten)
{
	if (snd->voiceNumber < 0)
	{
		return;
	}
	LPDIRECTSOUNDBUFFER* bufferPtr = &soundDatas[snd->voiceNumber].buffer;
	if (DS_OK == (*bufferPtr)->Unlock(buffer, bytesWritten, NULL, 0))
	{
		// ok
	}
	// TODO try to Restore
}
static Sound LoadWav(Sound s, const char* filename)
{
	Log_InfoF("Loading Wav Sound to DirectSound from %s\n", filename);

	// Open the WAV file
	SF_INFO sfinfo;
	SNDFILE* sndfile = sf_open(filename, SFM_READ, &sfinfo);
	if (!sndfile) {
		OutputDebugStringA("SNDFile failed to open file\n");
		return s;
	}

	sizetype dataSize = sfinfo.frames * sfinfo.channels * sizeof(s16);
	void* buffer = Audio_OpenStaticBuffer(&s, dataSize, sfinfo.samplerate, Sound_Stereo_s16);
	sf_read_raw(sndfile, buffer, dataSize);
	Audio_CloseStaticBuffer(&s, buffer, dataSize);
	sf_close(sndfile);

	return s;
}

void Audio_Platform_PlaySound(Sound* snd)
{
	// TODO : if fails, try to Restore buffer
	soundDatas[snd->voiceNumber].buffer->Play(0, 0, 0);
}
sizetype Audio_GetStaticBufferSize(Sound* snd)
{
	return soundDatas[snd->voiceNumber].sizeBytes;
}

void Audio_Platform_UnloadSound(Sound s)
{
	if (soundDatas[s.voiceNumber].buffer != nullptr)
	{
		soundDatas[s.voiceNumber].buffer->Stop();
		soundDatas[s.voiceNumber].buffer->Release();
	}
}

void Audio_Platform_StartStream(Sound* snd, s32 sampleRate)
{
	activeStreamingSound = snd->voiceNumber;
	streamingBuffer->SetFrequency(sampleRate);
	WriteToStream(streamingBufferSize / 2); // Write half the buffer in advance
	streamingBuffer->Play(0,0,DSBPLAY_LOOPING);
}
void Audio_Platform_StopStream(s32 voiceNumber)
{
	activeStreamingSound = -1;
	streamingBuffer->Stop();
}

bool Create_Buffer(u32 sizeBytes, WORD channels, LPDIRECTSOUNDBUFFER* bufferPtr)
{
	Log_InfoF("Create buffer bytes %u, channels %d\n", sizeBytes, channels);
	// Create a write buffer  for music playback
	WAVEFORMATEX format = waveFormatPrimary;
	format.nChannels = channels;
	format.nBlockAlign = (format.nChannels * format.wBitsPerSample) / 8; // 8 is bits per byte
	format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;

	DSBUFFERDESC writeBufferDescription = { 0 };
	writeBufferDescription.dwSize = sizeof(DSBUFFERDESC); // Size of the structure
	writeBufferDescription.dwFlags = 0;
	writeBufferDescription.dwBufferBytes = sizeBytes;
	writeBufferDescription.dwReserved = 0; // Must be 0
	writeBufferDescription.lpwfxFormat = &format; // Format, NULL for write
	writeBufferDescription.guid3DAlgorithm = DS3DALG_DEFAULT; // Virtualization algo

	// Create a write buffer
	HRESULT createWriteBufferResult = DirectSoundObject->CreateSoundBuffer(&writeBufferDescription, bufferPtr, NULL);
	if (createWriteBufferResult != DS_OK)
	{
		// Failed to create write buffer
		Log_Error("Failed to create write buffer");
		OutputDebugStringA("Failed to create write buffer\n");
		switch (createWriteBufferResult)
		{
		case	DSERR_ALLOCATED:
			OutputDebugStringA("ALLOCATED\n"); break;
		case	DSERR_CONTROLUNAVAIL:
			OutputDebugStringA("CONTROLUNAVAIL\n"); break;
		case	DSERR_BADFORMAT:
			OutputDebugStringA("BADFORMAT\n"); break;
		case	DSERR_INVALIDPARAM:
			OutputDebugStringA("INVALIDPARAM\n"); break;
		case	DSERR_NOAGGREGATION:
			OutputDebugStringA("NOAGGREGATION\n"); break;
		case	DSERR_OUTOFMEMORY:
			OutputDebugStringA("OUTOFMEMORY\n"); break;
		case	DSERR_UNINITIALIZED:
			OutputDebugStringA("UNINITIALIZED\n"); break;
		case	DSERR_UNSUPPORTED:
			OutputDebugStringA("UNSUPPORTED\n"); break;
		};

		return false;
	}
	return true;
}


void Audio_Update(void)
{
	// Check if streaming is active
	if (activeStreamingSound < 0)
	{
		return;
	}

	DWORD statusFlagsOut;
	streamingBuffer->GetStatus(&statusFlagsOut);
	if (Flag_IsSet(statusFlagsOut, DSBSTATUS_PLAYING) == false)
	{
		// Streaming sound is not playing
		return;
	}

	// Check if the play cursor has advanced from last time
	// that means we can write more audio
	DWORD playposition;
	DWORD writeposition;
	HRESULT positionResult = streamingBuffer->GetCurrentPosition(&playposition, &writeposition);

	//Log_InfoF("PlayCursor %d, last play %d / size %d\n", playposition, lastPlayCursorPosition, streamingBufferSize);
	//Log_InfoF("WriteCursors: system %d, own %d\n", writeposition, lastWritePosition);
	DWORD playMoveBytes = 0;
	if (playposition > lastPlayCursorPosition)
	{
		// The play cursor has moved ahead from last time
		playMoveBytes = playposition - lastPlayCursorPosition;
	}
	else if (playposition < lastPlayCursorPosition)
	{
		// The play cursor has moved ahead and looped around
		// From last position to end of buffer
		playMoveBytes = streamingBufferSize - lastPlayCursorPosition;
		// From start of buffer to play position
		playMoveBytes += playposition;
	}

	/*
	DWORD writeMoveBytes = 0;
	if (writeposition > lastWriteCursorPosition)
	{
		// The play cursor has moved ahead from last time
		writeMoveBytes = writeposition - lastWriteCursorPosition;
	}
	else if (writeposition < lastWriteCursorPosition)
	{
		// The play cursor has moved ahead and looped around
		// From last position to end of buffer
		writeMoveBytes = streamingBufferSize - lastWriteCursorPosition;
		// From start of buffer to play position
		writeMoveBytes += writeposition;
	}
	*/
	if (playMoveBytes > 0)
	{
		lastPlayCursorPosition = playposition;
		lastWriteCursorPosition = writeposition;
		DWORD bytesToWrite = playMoveBytes;
		WriteToStream(bytesToWrite);
	}
}

static void WriteToStream(DWORD bytesToWrite)
{
	// Start writing to buffer
	VOID* writeLocation1;
	VOID* writeLocation2;
	DWORD location1Size;
	DWORD location2Size;
	DWORD flags = 0;// DSBLOCK_FROMWRITECURSOR;
	// Lock the buffer before copying data to it
	// This functions returns 1 or 2 locations for writing

	HRESULT lockResult = streamingBuffer->Lock(
		lastWritePosition, 
		bytesToWrite,
		&writeLocation1,
		&location1Size,
		&writeLocation2,
		&location2Size,
		flags);

	if (lockResult != DS_OK)
	{
		return;
	}

	// Call the callback function
	s16* sampleOut = (s16*)writeLocation1;
	u32 bytesWritten1 = 0;
	u32 bytesWritten2 = 0;
	//Log_InfoF("Streaming callback asks for %d bytes\n", location1Size);
	audioCallback(activeStreamingSound, sampleOut, location1Size, &bytesWritten1);
	//Log_InfoF("Streaming callback got %d bytes\n", bytesWritten1);
	if (bytesWritten1 < location1Size)
	{
		location1Size = bytesWritten1;
	}
	lastWritePosition += bytesWritten1;

	if (writeLocation2 != NULL && location2Size > 0)
	{
		// 2 locations
		sampleOut = (s16*)writeLocation2;
		// Log_InfoF("Streaming callback asks for %d bytes\n", location2Size);
		audioCallback(activeStreamingSound, sampleOut, location2Size, &bytesWritten2);
		//Log_InfoF("Streaming callback got %d bytes\n", bytesWritten2);
		if (bytesWritten2 < location2Size)
		{
			location2Size = bytesWritten2;
		}
		lastWritePosition += bytesWritten1;
	}
	// Write done, unlock
	HRESULT unlockResult = streamingBuffer->Unlock(writeLocation1, location1Size, writeLocation2, location2Size);

	lastWritePosition = lastWritePosition % streamingBufferSize;
}

void Audio_Platform_Deinit(void)
{
	for (int i = 0; i < MGDL_AUDIO_MAX_VOICES; i++)
	{
		if (soundDatas[i].buffer != nullptr)
		{
			soundDatas[i].buffer->Stop();
			soundDatas[i].buffer->Release();
		}
	}
	streamingBuffer->Stop();
	streamingBuffer->Release();

	DirectSoundPrimaryBuffer->Stop();
	DirectSoundPrimaryBuffer->Release();
	
	DirectSoundObject->Release();

}
void Audio_SetPaused(bool paused)
{
}
bool Audio_IsPaused(void)
{
}

/**
@brief Stops the given voice
@param Number of the voice
@return True if voice was stopped
*/
bool Audio_StopSound(Sound* snd) 
{
	if (snd->type == SoundWav)
	{
		soundDatas[snd->voiceNumber].buffer->Stop();
	}
	else if (snd->type == SoundOgg && snd->voiceNumber == activeStreamingSound)
	{
		streamingBuffer->Stop();
	}

	return true;
}
/**
@brief Pauses the given voice
@param Number of the voice
@return True if voice was paused
*/
bool Audio_PauseSound(Sound* snd)
{
	if (snd != nullptr && snd->voiceNumber >= 0 && snd->voiceNumber < MGDL_AUDIO_MAX_VOICES)
	{
		DWORD statusFlagsOut;
		soundDatas[snd->voiceNumber].buffer->GetStatus(&statusFlagsOut);
		if (Flag_IsSet(statusFlagsOut,DSBSTATUS_PLAYING))
		{
			soundDatas[snd->voiceNumber].buffer->Stop();
			return true;
		}
	}
	return false;

}
/**
@brief Gets the status of the given voice
@param Number of the voice
@return Status of the voice, or Invalid if the voice number is not in use
*/
mgdlAudioStateEnum Audio_GetSoundStatus(Sound* snd)
{
	if (snd != nullptr && snd->voiceNumber >= 0 && snd->voiceNumber < MGDL_AUDIO_MAX_VOICES)
	{
		DWORD statusFlagsOut;
		soundDatas[snd->voiceNumber].buffer->GetStatus(&statusFlagsOut);
		if (Flag_IsSet(statusFlagsOut,DSBSTATUS_PLAYING))
		{
			return Audio_StatePlaying;
		}
		else
		{
			return Audio_StateStopped;
		}
	}
	return Audio_StateInvalid;
}

/**
@brief Sets volume for given voice
@param Number of the voice
@param normalizedVolume New volume between 0.0f and 1.0f
*/
mgdlAudioStateEnum Audio_SetVoiceVolume(s32 voiceNumber, float normalizedVolume)
{

}
/**
@brief Returns how many milliseconds the given voice has been playing
@param Number of the voice
@return Elapsed playback duration in milliseconds
*/
u32 Audio_GetSoundElapsedMs(Sound* snd)
{
	if (snd != nullptr && snd->voiceNumber > 0 && snd->voiceNumber < MGDL_AUDIO_MAX_VOICES)
	{
		DWORD playposition;
		DWORD writeposition;
		HRESULT positionResult = soundDatas[snd->voiceNumber].buffer->GetCurrentPosition(&playposition, &writeposition);
		if (positionResult == DS_OK)
		{
			// calculate to ms      bytes /  ms in second * samples per second * (bytes per sample)
			DWORD samplesElapsed = playposition / (soundDatas[snd->voiceNumber].channels);
			DWORD msElapsed = (playposition * 1000) / MGDL_AUDIO_SAMPLE_RATE;
			return msElapsed;
		}
	}
	return 0;
}

void Audio_SetSoundElapsedMs(Sound* snd, s32 milliseconds)
{
	if (snd->type == SoundWav)
	{
		if (snd != nullptr && snd->voiceNumber > 0 && snd->voiceNumber < MGDL_AUDIO_MAX_VOICES)
		{
			// Milliseconds to bytes
			// TODO What if different sample rate?
			DWORD bytesSecond = (soundDatas[snd->voiceNumber].channels) * sizeof(s16) * MGDL_AUDIO_SAMPLE_RATE;
			float secondsElapsed = ((float)milliseconds / 1000.0f);
			DWORD bytesElapsed = secondsElapsed * bytesSecond;

			HRESULT positionResult = soundDatas[snd->voiceNumber].buffer->SetCurrentPosition(bytesElapsed);
		}
	}
	else if (snd->type == SoundOgg)
	{
		if (snd != nullptr && snd->voiceNumber > 0 && snd->voiceNumber < MGDL_AUDIO_MAX_MUSIC)
		{
			

		}
	}
}
#endif
