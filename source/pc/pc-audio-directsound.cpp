#include <mgdl/mgdl-audio.h>
#if defined(MGDL_WINDOWS_NATIVE)

#include <mgdl/mgdl-logger.h>
#include <Dsound.h>

// Pointer to Object created by DirectSoundCreate
static LPDIRECTSOUND DirectSoundObject = nullptr;
// Pointer to Buffer created by direct sound
static LPDIRECTSOUNDBUFFER DirectSoundPrimaryBuffer = nullptr;

// One buffer for music
static LPDIRECTSOUNDBUFFER DirectSoundWriteBuffer = nullptr;
static DWORD writeBufferSize = 0;
static DWORD lastWriteCursorPosition = 0;
static DWORD lastPlayCursorPosition = 0;

static LPDIRECTSOUNDBUFFER* soundBuffers;

// Multiple buffers for sound effects

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

void Audio_Init(void* platformData)
{
	HWND windowHandle = *(HWND*)platformData;
	OutputDebugStringA("Initializing DirectSound\n");

	soundBuffers = (LPDIRECTSOUNDBUFFER*)malloc(sizeof(LPDIRECTSOUNDBUFFER) * MGDL_AUDIO_MAX_VOICES);

	// Init windows Multimedia audio system
	// Load the library for DirectSound audio
	HMODULE DirectSoundLibrary = LoadLibraryA("dsound.dll");;
	if (DirectSoundLibrary == NULL)
	{
		Log_Error("Failed load dsound.dll");
		OutputDebugStringA("Failed load dsound.dll");
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
		HRESULT cooperativeResult = DirectSoundObject->SetCooperativeLevel(windowHandle, DSSCL_PRIORITY);
		if (cooperativeResult != DS_OK)
		{
			// Failed to set cooperative level
			Log_Error("Failed to set cooperative level");
			OutputDebugStringA("Failed to set cooperative level");

			return;
		}
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
			Log_Error("Failed to create primary buffer");
			OutputDebugStringA("Failed to create primary buffer");

			return;
		}
		WAVEFORMATEX waveFormatPrimary = { 0 };
		waveFormatPrimary.wFormatTag = WAVE_FORMAT_PCM;
		waveFormatPrimary.nChannels = 2;
		waveFormatPrimary.nSamplesPerSec = 44100;
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

		// Create a write buffer  for music playback
		writeBufferSize= waveFormatPrimary.nAvgBytesPerSec * 2;

		DSBUFFERDESC writeBufferDescription = { 0 };
		writeBufferDescription.dwSize = sizeof(DSBUFFERDESC); // Size of the structure
		writeBufferDescription.dwFlags = 0;
		// Audio for 2 seconds
		writeBufferDescription.dwBufferBytes = writeBufferSize;
		writeBufferDescription.dwReserved = 0; // Must be 0
		writeBufferDescription.lpwfxFormat = &waveFormatPrimary; // Format, NULL for write
		writeBufferDescription.guid3DAlgorithm = DS3DALG_DEFAULT; // Virtualization algo

		// Create a write buffer
		HRESULT createWriteBufferResult = DirectSoundObject->CreateSoundBuffer(&writeBufferDescription, &DirectSoundWriteBuffer, NULL);
		if (createWriteBufferResult != DS_OK)
		{
			// Failed to create write buffer
			Log_Error("Failed to create write buffer");
			OutputDebugStringA("Failed to create write buffer");
			return;
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


void Audio_Update(void)
{

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
@brief Tells the audio system to start a new voice
@return The voice number for the started voice, this will be sent to the callback function when more audio system needs more samples. If the return value is -1 it means a new voice could not be started
*/
s32 Audio_StartNewVoice(void)
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
mgdlAudioStateEnum Audio_GetVoiceStatus(s32 voiceNumber)
{

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
u32 Audio_GetVoiceElapsedMs(s32 voiceNumber)
{

}
#endif
