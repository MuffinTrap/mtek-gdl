#if defined(MGDL_PLATFORM_WINDOWS)
#include <mgdl/pc/mgdl-audio-directsound.h>

#include <mgdl/mgdl-audio.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-util.h>

// Pointer to Object created by DirectSoundCreate
static LPDIRECTSOUND DirectSoundObject = nullptr;
// Pointer to Buffer created by direct sound
static LPDIRECTSOUNDBUFFER DirectSoundPrimaryBuffer = nullptr;

// Main buffer format
static WAVEFORMATEX waveFormatPrimary = { 0 };
static WAVEFORMATEX waveFormatStreaming = { 0 };

// One streaming buffer for music
static AudioCallbackFunction audioCallback = nullptr;
static LPDIRECTSOUNDBUFFER streamingBuffer = nullptr;
static DWORD streamingBufferSize = 0;
static DWORD lastWriteCursorPosition = -MGDL_AUDIO_CALLBACK_BUFFER_SIZE;
static DWORD lastPlayCursorPosition = 0;
static DWORD lastWritePosition = 0; // Make sure to always continue writing from the last point
static void WriteToStream(DWORD bytesToWrite);

// Multiple buffers for music and sound effects
static SoundDirectSound* soundDatas;
static bool Create_Buffer(DWORD sizeBytes, SoundSampleFormat format, u32 sampleRate, DWORD flags, LPDIRECTSOUNDBUFFER* bufferPtr, bool isStreamingBuffer);

// static HANDLE soundBufferEmptyEvent;

#define DIRECT_SOUND_CREATE(name) HRESULT WINAPI name(LPGUID deviceGUID, LPDIRECTSOUND* objectAddress, LPUNKNOWN aggregate)

typedef DIRECT_SOUND_CREATE(dsound_create);

DIRECT_SOUND_CREATE(DirectSoundCreateStub)
{
	return DSERR_NODRIVER;
}

static HRESULT(*DirectSound_Create_FuncP)(LPGUID deviceGUID, LPDIRECTSOUND* objectAddress, LPUNKNOWN aggregate) = DirectSoundCreateStub;

static void PrintDirectSoundError(HRESULT error)
{
		switch (error)
		{
		case	DSERR_ALLOCATED:
			Log_Error("ALLOCATED\n"); break;
		case	DSERR_CONTROLUNAVAIL:
			Log_Error("CONTROLUNAVAIL\n"); break;
		case	DSERR_BADFORMAT:
			Log_Error("BADFORMAT\n"); break;
		case	DSERR_INVALIDPARAM:
			Log_Error("INVALIDPARAM\n"); break;
		case	DSERR_NOAGGREGATION:
			Log_Error("NOAGGREGATION\n"); break;
		case	DSERR_OUTOFMEMORY:
			Log_Error("OUTOFMEMORY\n"); break;
		case	DSERR_UNINITIALIZED:
			Log_Error("UNINITIALIZED\n"); break;
		case	DSERR_UNSUPPORTED:
			Log_Error("UNSUPPORTED\n"); break;
		};

}


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

	soundDatas = (SoundDirectSound*)malloc(sizeof(SoundDirectSound) * MGDL_AUDIO_MAX_SOUNDS);
	for (int i = 0; i < MGDL_AUDIO_MAX_SOUNDS; i++)
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
			PrintDirectSoundError(DCreateResult);

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
			PrintDirectSoundError(cooperativeResult);

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
			PrintDirectSoundError(createBufferResult);

			return;
		}
		waveFormatPrimary = { 0 };
		waveFormatPrimary.wFormatTag = WAVE_FORMAT_PCM;
		waveFormatPrimary.nChannels = 2;
		waveFormatPrimary.nSamplesPerSec = MGDL_AUDIO_SAMPLE_RATE_48K;
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
			PrintDirectSoundError(setPrimaryFormatResult);
		}

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

static LPVOID static_lpWrite2;
static DWORD static_dwLength2;

void* Audio_OpenStaticBuffer(Sound* inout_snd, sizetype byteCount, u16 samplerate, SoundSampleFormat format)
{
	Log_InfoF("Open Buffer byteCount %zu, samplerate %d\n", byteCount, samplerate);
	// Find first free sound
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
		return nullptr;
	}

	DWORD sizeBytes = (DWORD)byteCount;
	LPDIRECTSOUNDBUFFER* bufferPtr = &soundDatas[voiceNumber].buffer;
	if (Create_Buffer(sizeBytes, format, samplerate, 0, bufferPtr, false))
	{
		// Lock, Fill and Unlock buffer
		LPVOID lpWrite;
		DWORD dwLength;
		HRESULT lockResult = (*bufferPtr)->Lock(
			0,
			0,
			&lpWrite,
			&dwLength,
			&static_lpWrite2,
			&static_dwLength2,
			DSBLOCK_ENTIREBUFFER);
		if (lockResult == DS_OK)
		{
			//soundDatas[voiceNumber].buffer->Unlock(lpWrite, dwLength, static_lpWrite2, static_dwLength2);
			soundDatas[voiceNumber].channels = Sound_FormatToChannels(format);
			soundDatas[voiceNumber].inUse = true;
			soundDatas[voiceNumber].sizeBytes = sizeBytes;
			soundDatas[voiceNumber].elapsedSeconds = 0.0f;
			soundDatas[voiceNumber].samplerate = samplerate;

			// Set common values and return
			inout_snd->voiceNumber = voiceNumber;
			

			Log_InfoF("Opened buffer %d for writing %d, bytes %d\n", voiceNumber, dwLength);
			return lpWrite;
		}
		else
		{
			OutputDebugStringA("Failed to lock buffer\n");
			Log_Error("Failed to lock buffer\n");
			PrintDirectSoundError(lockResult);
		}

		// TODO : if fails, try to Restore buffer
	}
	else
	{
		OutputDebugStringA("Failed to create buffer\n");
		Log_Error("Failed to create buffer\n");
	}
	return nullptr;
}

void Audio_CloseStaticBuffer(Sound* snd, void* buffer, sizetype bytesWritten)
{
	Log_InfoF("Close Buffer bytesWritten %zu, pointer %p\n", bytesWritten, buffer);
	if (snd->voiceNumber < 0)
	{
		Log_ErrorF("Cannot close buffer, invalid id : %d\n", snd->voiceNumber);
		return;
	}
	LPDIRECTSOUNDBUFFER* bufferPtr = &soundDatas[snd->voiceNumber].buffer;
	HRESULT unlockResult = (*bufferPtr)->Unlock(buffer, (DWORD)bytesWritten, static_lpWrite2, static_dwLength2);
	if (unlockResult == DS_OK)
	{
		// ok
		Log_InfoF("Closed buffer %d for writing\n", snd->voiceNumber);
	}
	else
	{
		Log_ErrorF("Failed to close buffer %d for writing\n", snd->voiceNumber);
		PrintDirectSoundError(unlockResult);
	}
}
static Sound LoadWav(Sound s, const char* filename)
{
	Log_InfoF("Loading Wav Sound to DirectSound from %s\n", filename);

	/*
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
	*/

	return s;
}

void Audio_PlayStaticBuffer(Sound* snd)
{
	// TODO : if fails, try to Restore buffer
	soundDatas[snd->voiceNumber].buffer->Play(0, 0, 0);
}
void Audio_PauseStaticBuffer(Sound* snd, bool paused)
{
	if (snd != nullptr && snd->voiceNumber >= 0 && snd->voiceNumber < MGDL_AUDIO_MAX_SOUNDS)
	{
		if (paused)
		{
			DWORD statusFlagsOut;
			soundDatas[snd->voiceNumber].buffer->GetStatus(&statusFlagsOut);
			if (Flag_IsSet(statusFlagsOut, DSBSTATUS_PLAYING))
			{
				soundDatas[snd->voiceNumber].buffer->Stop();
			}
		}
		else
		{
			soundDatas[snd->voiceNumber].buffer->Play(0, 0, 0);
		}
	}
}
void Audio_StopStaticBuffer(Sound* snd)
{
	// TODO : if fails, try to Restore buffer
	soundDatas[snd->voiceNumber].buffer->Stop();
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
// TODO What if different format than before?
void Audio_Platform_StartStream(Sound* snd, s32 sampleRate, SoundSampleFormat format)
{
	if (activeStreamingSound != snd->voiceNumber)
	{
		if (streamingBuffer != nullptr)
		{
			// Stop previous stream
			streamingBuffer->Stop();
		}
		activeStreamingSound = -1;
	}

	// Does the buffer exist?
	if (streamingBuffer == nullptr)
	{
		// Create streaming buffer for voice number 0 that is the music.
		OutputDebugStringA("Creating streaming buffer\n");
		Log_Info("Creating streaming buffer\n");
		Create_Buffer(0, 
			format,
			sampleRate,
			0, 
			&streamingBuffer,
			true);
	}
	// Can the existing buffer be used?
	else if (waveFormatStreaming.nSamplesPerSec != sampleRate)
	{
		streamingBuffer->Release();
		Create_Buffer(0, 
			format,
			sampleRate,
			0, 
			&streamingBuffer,
			true);
	}

	activeStreamingSound = snd->voiceNumber;
	WriteToStream(streamingBufferSize / 2); // Write half the buffer in advance
	streamingBuffer->Play(0,0,DSBPLAY_LOOPING);
}


static WORD SampleFormatToBitsPerSample(SoundSampleFormat sampleFormat)
{
	switch (sampleFormat)
	{
	case Format_Mono_16:
	case Format_Stereo_16:
		return 16;
	case Format_Mono_8:
	case Format_Stereo_8:
		return 8;
	}
	return 0;
}

bool Create_Buffer(DWORD sizeBytes, SoundSampleFormat sampleFormat, u32 sampleRate, DWORD flags, LPDIRECTSOUNDBUFFER* bufferPtr, bool isStreamingBuffer)
{
	Log_InfoF("Create buffer samplerate %d bytes %u, channels %d\n", sampleRate, sizeBytes, Sound_FormatToChannels(sampleFormat));
	// Create a write buffer  for music playback
	WAVEFORMATEX format = waveFormatPrimary;
	format.nSamplesPerSec = sampleRate;
	format.nChannels = Sound_FormatToChannels(sampleFormat);
	format.wBitsPerSample = SampleFormatToBitsPerSample(sampleFormat);
	format.nBlockAlign = (format.nChannels * format.wBitsPerSample) / 8; // 8 is bits per byte
	format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;

	// If this is the streaming buffer,
	// calculate size now
	DWORD bufferCreateSize = 0;
	if (isStreamingBuffer)
	{
		streamingBufferSize = format.nAvgBytesPerSec * 2;
		waveFormatStreaming = format;
		bufferCreateSize = streamingBufferSize;
		Log_InfoF("This is a streaming buffer. bytes %u\n", sizeBytes);
	}
	else
	{
		bufferCreateSize = sizeBytes;
	}

	DSBUFFERDESC writeBufferDescription = { 0 };
	writeBufferDescription.dwSize = sizeof(DSBUFFERDESC); // Size of the structure
	writeBufferDescription.dwFlags = flags;
	writeBufferDescription.dwBufferBytes = bufferCreateSize;
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
		PrintDirectSoundError(createWriteBufferResult);

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
	if (streamingBuffer == nullptr)
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
	for (int i = 0; i < MGDL_AUDIO_MAX_SOUNDS; i++)
	{
		if (soundDatas[i].buffer != nullptr)
		{
			soundDatas[i].buffer->Stop();
			soundDatas[i].buffer->Release();
		}
	}
	if (streamingBuffer != nullptr)
	{
		streamingBuffer->Stop();
		streamingBuffer->Release();
		streamingBuffer = nullptr;
	}

	DirectSoundPrimaryBuffer->Stop();
	DirectSoundPrimaryBuffer->Release();
	
	DirectSoundObject->Release();
}

void Audio_SetPaused(bool paused)
{
	// TODO
}
bool Audio_IsPaused(void)
{
	// TODO
	return false;
}

void Audio_Platform_PauseStream(Sound* snd)
{
	if (snd->voiceNumber == activeStreamingSound)
	{
		streamingBuffer->Stop();
	}
}

void Audio_Platform_ResumeStream(Sound* snd)
{
	if (snd->voiceNumber == activeStreamingSound)
	{
		streamingBuffer->Play(0,0,DSBPLAY_LOOPING);
	}
}

void Audio_Platform_StopStream(Sound* snd) 
{
	if (snd->voiceNumber == activeStreamingSound)
	{
		streamingBuffer->Stop();
		activeStreamingSound = -1;
	}
}

mgdlAudioStateEnum Audio_GetStaticBufferStatus(Sound* snd)
{
	if (snd != nullptr && snd->voiceNumber >= 0 && snd->voiceNumber < MGDL_AUDIO_MAX_SOUNDS)
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

mgdlAudioStateEnum Audio_SetVoiceVolume(s32 voiceNumber, float normalizedVolume)
{
	return Audio_StateInvalid;
}

void Audio_SetStaticBufferElapsedMs(Sound* snd, u32 milliseconds)
{
	if (snd != nullptr && snd->voiceNumber > 0 && snd->voiceNumber < MGDL_AUDIO_MAX_SOUNDS)
	{
		if (snd->type == SoundWav)
		{
			// Milliseconds to bytes
			// TODO What if different sample rate?
			DWORD bytesSecond = (soundDatas[snd->voiceNumber].channels) * sizeof(s16) * soundDatas[snd->voiceNumber].samplerate;
			float secondsElapsed = ((float)milliseconds / 1000.0f);
			DWORD bytesElapsed = (DWORD)(secondsElapsed * bytesSecond);

			HRESULT positionResult = soundDatas[snd->voiceNumber].buffer->SetCurrentPosition(bytesElapsed);
		}
	}
}

u32 Audio_GetStaticBufferElapsedMs(Sound* snd)
{
	if (snd != nullptr && snd->voiceNumber >= 0 && snd->voiceNumber < MGDL_AUDIO_MAX_SOUNDS)
	{
		DWORD playposition;
		DWORD writeposition;
		
		HRESULT positionResult = soundDatas[snd->voiceNumber].buffer->GetCurrentPosition(&playposition, &writeposition);
		if (positionResult == DS_OK)
		{
			// calculate to ms      bytes /  ms in second * samples per second * (bytes per sample)
			DWORD samplesElapsed = playposition / (soundDatas[snd->voiceNumber].channels);
			DWORD msElapsed = (playposition * 1000) / soundDatas[snd->voiceNumber].samplerate;
			return msElapsed;
		}
	}
	return 0;
}

#endif
