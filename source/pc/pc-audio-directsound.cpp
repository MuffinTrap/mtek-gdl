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

// One buffer for music
static LPDIRECTSOUNDBUFFER DirectSoundWriteBuffer = nullptr;
static DWORD writeBufferSize = 0;
static DWORD lastWriteCursorPosition = 0;
static DWORD lastPlayCursorPosition = 0;

// Multiple buffers for music and sound effects
static SoundDirectSound* soundDatas;
static bool Create_Buffer(u32 sizeBytes, WORD channels, u32 index);

AudioCallbackFunction audioCallback = nullptr;
// static HANDLE soundBufferEmptyEvent;

#define DIRECT_SOUND_CREATE(name) HRESULT WINAPI name(LPGUID deviceGUID, LPDIRECTSOUND* objectAddress, LPUNKNOWN aggregate)

typedef DIRECT_SOUND_CREATE(dsound_create);

DIRECT_SOUND_CREATE(DirectSoundCreateStub)
{
	return DSERR_NODRIVER;
}

static HRESULT(*DirectSound_Create_FuncP)(LPGUID deviceGUID, LPDIRECTSOUND* objectAddress, LPUNKNOWN aggregate) = DirectSoundCreateStub;


// Playback and audio state

static DWORD ringBufferWritePoint;

void Audio_SetCallback(AudioCallbackFunction callbackFunction)
{

}

void Audio_Platform_Init(void* platformData)
{
	HWND windowHandle = *(HWND*)platformData;
	OutputDebugStringA("Initializing DirectSound\n");

	soundDatas = (SoundDirectSound*)malloc(sizeof(SoundDirectSound) * MGDL_AUDIO_MAX_VOICES);

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
		writeBufferSize = waveFormatPrimary.nAvgBytesPerSec * 2;
		Create_Buffer(writeBufferSize, 2, MGDL_AUDIO_MUSIC_NUMBER);

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

Sound Audio_Platform_LoadSound(const char* filename, s32 voiceNumber)
{
	Sound s;
	s.sizeBytes = 0;
	s.voiceNumber = -1;

	if (voiceNumber <= MGDL_AUDIO_MUSIC_NUMBER)
	{
		return s;
	}
	Log_InfoF("Loading Sound to DirectSound from %s to voice %d\n", filename, voiceNumber);


	// Open the WAV file
	SF_INFO sfinfo;
	SNDFILE* sndfile = sf_open(filename, SFM_READ, &sfinfo);
	if (!sndfile) {
		OutputDebugStringA("SNDFile failed to open file\n");
		return s;
	}

	sizetype dataSize = sfinfo.frames * sfinfo.channels * sizeof(s16);
	u32 sizeBytes = dataSize;
	if (Create_Buffer(sizeBytes, sfinfo.channels, voiceNumber))
	{
		// Lock, Fill and Unlock buffer
		LPVOID lpWrite;
		DWORD dwLength;
		if (DS_OK == soundDatas[voiceNumber].buffer->Lock(
			0,
			0,
			&lpWrite,
			&dwLength,
			NULL,
			NULL,
			DSBLOCK_ENTIREBUFFER))
		{
			sf_read_raw(sndfile, lpWrite, dwLength);
			soundDatas[voiceNumber].buffer->Unlock(lpWrite, dwLength, 0, 0);

			soundDatas[voiceNumber].channels = sfinfo.channels;

			// Set common values and return
			s.voiceNumber = voiceNumber;
			s.sizeBytes = sizeBytes;
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
	sf_close(sndfile);
	return s;
}

void Audio_Platform_PlaySound(s32 voiceNumber)
{
	// TODO : if fails, try to Restore buffer
	soundDatas[voiceNumber].buffer->Play(0, 0, 0);
}

void Audio_Platform_UnloadSound(Sound s)
{
	// ... TODO free buffer
}

bool Create_Buffer(u32 sizeBytes, WORD channels, u32 index)
{
	Log_InfoF("Create buffer bytes %u, channels %d, index %d\n", sizeBytes, channels, index);
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
	HRESULT createWriteBufferResult = DirectSoundObject->CreateSoundBuffer(&writeBufferDescription, &soundDatas[index].buffer, NULL);
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
	// Check if voice 0 is playing
	DWORD statusFlagsOut;
	soundDatas[MGDL_AUDIO_MUSIC_NUMBER].buffer->GetStatus(&statusFlagsOut);
	if (Flag_IsSet(statusFlagsOut, DSBSTATUS_PLAYING) == false)
	{
		// Not playing
		return;
	}

	// Check if there is enough space in the buffer to write
	DWORD playposition;
	DWORD writeposition;
	HRESULT positionResult = DirectSoundWriteBuffer->GetCurrentPosition(&playposition, &writeposition);

	DWORD spaceBytes = 0;
	if (playposition > lastPlayCursorPosition)
	{
		// The play cursor has moved ahead from last time
		spaceBytes = playposition - lastPlayCursorPosition;
	}
	else if (playposition < lastPlayCursorPosition)
	{
		// The play cursor has moved ahead and looped around
		// From last position to end of buffer
		spaceBytes = writeBufferSize - lastPlayCursorPosition;
		// From start of buffer to play position
		spaceBytes += playposition;
	}
	if (spaceBytes < MGDL_AUDIO_CALLBACK_BUFFER_SIZE)
	{
		// Not enough space
		return;
	}

	// Start writing to buffer

	DWORD bytesToWrite = spaceBytes;
	VOID* writeLocation1;
	VOID* writeLocation2;
	LPVOID* write1Ptr = &writeLocation1;
	LPVOID* write2Ptr = &writeLocation2;
	DWORD location1Size;
	DWORD location2Size;
	DWORD flags = DSBLOCK_FROMWRITECURSOR;
	// Lock the buffer before copying data to it
	// This functions returns 1 or 2 locations for writing

	HRESULT lockResult = DirectSoundWriteBuffer->Lock(
		0, // The Lock is set automatically to write cursor position
		bytesToWrite,
		write1Ptr,
		&location1Size,
		write2Ptr,
		&location2Size,
		flags);


	if (lockResult != DS_OK)
	{
		return;
	}

	// Call the callback function
	s16* sampleOut = (s16*)write1Ptr;
	u32 bytesWritten1 = 0;
	u32 bytesWritten2 = 0;
	audioCallback(0, sampleOut, location1Size, &bytesWritten1);

	if (bytesWritten1 < location1Size)
	{
		location1Size = bytesWritten1;
	}

	if (location1Size < bytesToWrite)
	{
		// 2 locations
		sampleOut = (s16*)write2Ptr;
		audioCallback(0, sampleOut, location2Size, &bytesWritten2);
		if (bytesWritten2 < location2Size)
		{
			location2Size = bytesWritten2;
		}
	}
	// Write done, unlock
	HRESULT unlockResult = DirectSoundWriteBuffer->Unlock(write1Ptr, location1Size, write2Ptr, location2Size);

}
void Audio_Deinit(void)
{

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
bool Audio_StopVoice(s32 voiceNumber) {

}
/**
@brief Pauses the given voice
@param Number of the voice
@return True if voice was paused
*/
bool Audio_PauseVoice(s32 voiceNumber)
{

}
/**
@brief Gets the status of the given voice
@param Number of the voice
@return Status of the voice, or Invalid if the voice number is not in use
*/
mgdlAudioStateEnum Audio_GetSoundStatus(Sound* snd)
{
	if (snd != nullptr && snd->voiceNumber > MGDL_AUDIO_MUSIC_NUMBER)
	{
		DWORD statusFlagsOut;
		soundDatas[MGDL_AUDIO_MUSIC_NUMBER].buffer->GetStatus(&statusFlagsOut);
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
	if (snd != nullptr && snd->voiceNumber > MGDL_AUDIO_MUSIC_NUMBER)
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
#endif
