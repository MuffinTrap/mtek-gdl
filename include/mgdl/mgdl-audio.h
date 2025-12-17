#pragma once

#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-sound.h>

/**
 * @file mgdl-audio.h
 * @ingroup audio
 * @brief Functions to use the sound system of the platform
 * @details These functions are implemented differently on different platforms: Wii uses ASND, Linux and Mac use OpenAL and Windows uses DirectSound
 */

#ifdef __cplusplus
extern "C"
{
#endif
	enum mgdlAudioStateEnum
	{
		Audio_StatePlaying,
		Audio_StatePaused,
		Audio_StateStopped,
		Audio_StateInvalid
	};
	typedef enum mgdlAudioStateEnum mgdlAudioStateEnum;
	/**
	@brief The callback function type for audio system
	@param voiceNumber The number of voice that needs more data
	@param bufferPtr The buffer where the samples need to be written
	@param maxBytesToWrite Maximum amount of bytes to write. If less bytes are left in the audio data, fill the rest of the buffer with zeroes
	@param bytesWritten Out parameter to store the actual number of bytes that was written to buffer
	*/
	typedef void (*AudioCallbackFunction)(s32 voiceNumber, s16* bufferPtr, u32 maxBytesToWrite, u32* bytesWritten);

	/**
	@brief Sample rate used by mgdl. This is because ASND uses 48k
	*/
#	define MGDL_AUDIO_SAMPLE_RATE 48000
#	define MGDL_AUDIO_CALLBACK_BUFFER_SIZE 4096
#	define MGDL_AUDIO_MAX_VOICES 256 // How many voices can be loaded at the same time


	/**
	* @brief Initializes the common audio system
	* @param Pointer to platform specific data that the audio system needs. On windows this is the Window handle: HWND
	*/
	void Audio_Init(void* platformSpecificData);

	/**
	* @brief Initializes the platform specific audio system
	* @param Pointer to platform specific data that the audio system needs. On windows this is the Window handle: HWND
	*/
	void Audio_Platform_Init(void* platformSpecificData);

	/**
	* @brief Sets the audio callback function
	* @param The callback function the audio system should call when more sound data is needed
	*/
	void Audio_SetCallback(AudioCallbackFunction callbackFunction);

	/**
	* @brief Updates the audio system. Needs to be called each frame.
	*/
	void Audio_Update(void);
	/**
	* @brief Closes the audio system
	*/
	void Audio_Deinit(void);
	/**
	* @brief Pauses or unpauses the playback of audio
	* @param paused To pause all audio, set this to true. To unpause, set to false
	*/
	void Audio_SetPaused(bool paused);
	/**
	* @brief Tells if the audio system is currently paused
	* @param paused To pause all audio, set this to true. To unpause, set to false
	* @return True when audio is paused, false when not paused
	*/
	bool Audio_IsPaused(void);


	/**
	@brief Stops the given voice
	@param Number of the voice
	@return True if voice was stopped
	*/
	bool Audio_StopVoice(s32 voiceNumber);
	/**
	@brief Pauses the given voice
	@param Number of the voice
	@return True if voice was paused
	*/
	bool Audio_PauseVoice(s32 voiceNumber);
	/**
	@brief Gets the status of the given voice
	@param Number of the voice
	@return Status of the voice, or Invalid if the voice number is not in use
	*/
	mgdlAudioStateEnum Audio_GetVoiceStatus(s32 voiceNumber);

	/**
	@brief Sets volume for given voice
	@param Number of the voice
	@param normalizedVolume New volume between 0.0f and 1.0f
	*/
	mgdlAudioStateEnum Audio_SetVoiceVolume(s32 voiceNumber, float normalizedVolume);
	/**
	@brief Returns how many milliseconds the given voice has been playing
	@param Number of the voice
	@return Elapsed playback duration in milliseconds
	*/
	u32 Audio_GetVoiceElapsedMs(s32 voiceNumber);

	/**
	 * @brief Set global maximum volume of sounds
	 * @param normalizedVolume Volume from 0 to 1.0
	 */
void Audio_SetSoundMasterVolume(float normalizedVolume);
	/**
	 * @brief Set global maximum volume of music
	 * @param normalizedVolume Volume from 0 to 1.0
	 */
void Audio_SetMusicMasterVolume(float normalizedVolume);
	/**
	 * @brief Set global maximum volume of all sounds
	 * @param normalizedVolume Volume from 0 to 1.0
	 */
void Audio_SetMasterAudioVolume(float normalizedVolume);

	/**
	 * @brief Loads a sound from given file
	 * @param Name of the wav file
	 */
Sound* Audio_LoadSound(const char* filename);

Sound Audio_Platform_LoadSound(const char* filename, s32 voiceNumber);
void Audio_Platform_UnloadSound(Sound s);

void Audio_PlaySound(Sound* s);
void Audio_Platform_PlaySound(s32 voiceNumber);


#ifdef __cplusplus
}
#endif
