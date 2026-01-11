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

	/**
	* @brief Enumeration for state of any Sound.
	*/
	enum mgdlAudioStateEnum
	{
		Audio_StatePlaying,
		Audio_StatePaused,
		Audio_StateStopped,
		Audio_StateInvalid
	};
	typedef enum mgdlAudioStateEnum mgdlAudioStateEnum;

	/**
	@brief The callback function type for streaming music
	@param soundNumber The number of sound that needs more data
	@param bufferPtr The buffer where the samples need to be written
	@param bufferSizeBytes Maximum amount of bytes to write. If less bytes are left in the audio data, fill the rest of the buffer with zeroes
	@param bytesWritten[out] Out parameter to store the actual number of bytes that was written to buffer
	*/
	typedef void (*AudioCallbackFunction)(s32 soundNumber, s16* bufferPtr, u32 bufferSizeBytes, u32* bytesWritten);

	/**
	@brief Sample rate used by mgdl. This is because ASND uses 48k
	*/
#	define MGDL_AUDIO_SAMPLE_RATE_48K 48000
#	define MGDL_AUDIO_SAMPLE_RATE_44K 44100
#	define MGDL_AUDIO_CALLBACK_BUFFER_SIZE 4096
#	define MGDL_AUDIO_MAX_SOUNDS 256 // How many sounds can be loaded at the same time

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
	* @param The callback function the audio system calls when more streaming sound data is needed
	*/
	void Audio_Platform_SetCallback(AudioCallbackFunction callbackFunction);

	/**
	* @brief Updates the audio system. Needs to be called each frame.
	*/
	void Audio_Update(void);
	/**
	* @brief Closes the audio system
	*/
	void Audio_Deinit(void);
	void Audio_Platform_Deinit(void);
	/**
	* @brief Pauses or unpauses the playback of all audio
	* @param paused To pause all audio, set this to true. To unpause, set to false
	*/
	void Audio_SetPaused(bool paused);
	/**
	* @brief Tells if the audio system is currently paused
	* @return True when audio is paused, false when not paused
	*/
	bool Audio_IsPaused(void);


	/**
	* @brief Plays a given sound once
	* @param snd The sound to play
	*/
	void Audio_PlaySound(Sound* s);
	/**
	@brief Stops the given sound
	@param Sound The sound to stop
	*/
	void Audio_StopSound(Sound* snd);
	/**
	@brief Pauses the given sound
	@param Sound The sound 
	@return True if sound was paused
	*/
	bool Audio_PauseSound(Sound* snd);
	/**
	@brief Gets the status of the given sound
	@param Sound The sound 
	@return Status of the sound, or Invalid if the sound number is not in use
	*/
	mgdlAudioStateEnum Audio_GetSoundStatus(Sound* snd);

	/**
	@brief Sets volume for given sound
	@param Sound The sound 
	@param normalizedVolume New volume between 0.0f and 1.0f
	*/
	mgdlAudioStateEnum Audio_SetVoiceVolume(Sound* snd, float normalizedVolume);
	/**
	@brief Returns how many milliseconds the given sound has been playing
	@param Sound The sound 
	@return Elapsed playback duration in milliseconds
	*/
	u32 Audio_GetSoundElapsedMs(Sound* snd);

	/**
	@brief Sets the playback position of the sound to given milliseconds from beginning
	@param Sound The sound to adjust
	@param milliseconds The new playback position
	*/
	void Audio_SetSoundElapsedMs(Sound* snd, s32 milliseconds);

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
 *	@brief Loads a sound file.
 *	@details This function loads a sound file using the appropriate player depending on the type of the sound
 *
 *	@note This library only supports
 * uncompressed Microsoft WAV sound files with a sample
 * format of 8 or 16-bit and a sample frequency
 * of up to 48KHz, Mono or Stereo.
 * For music the library supports ogg.
 *
 *	@param[in]	fileName	File name of sound file to load.
 *	@param[in]	filetype	What kind of sound data, wav or ogg
 *
 *	@returns Pointer if the sound file was successfully loaded, otherwise a null pointer
 */
Sound* Audio_LoadSound(const char* filename, SoundFileType filetype);

// Static buffers used by short sounds

/**
* @brief Creates and opens a static buffer for sound data.
* @param inout_snd The sound associated with the buffer. The sound number is used select the buffer and the rest of the data is filled in.
* @param byteCount Size of the buffer in bytes
* @param samplerate Sample rate of the buffer
* @param format Format of the sound: stereo or mono and 8 or 16 bit
* @return Pointer for writing sound data. Returns null pointer if buffer could not be opened.
*/
void* Audio_OpenStaticBuffer(Sound* inout_snd, sizetype byteCount, u16 samplerate, SoundSampleFormat format);
/**
* @brief Returns the size of a static buffer in bytes.
* @param snd The sound associated with the buffer. The sound number is used to find the buffer.
* @return Size of the buffer in bytes. If buffer is not found returns 0.
*/
sizetype Audio_GetStaticBufferSize(Sound* snd);
mgdlAudioStateEnum Audio_GetStaticBufferStatus(Sound* snd);

/**
* @brief Closes and finalizes a static buffer after writing is done.
* @param snd The sound associated with the buffer.
* @param buffer Pointer to the buffer
* @param bytesWritten How many bytes were written to the buffer
*/
void Audio_CloseStaticBuffer(Sound* snd, void* buffer, sizetype bytesWritten);

/**
* @brief Plays a static buffer once.
* @param snd The sound associated with the buffer
*/
void Audio_PlayStaticBuffer(Sound* snd);
/**
* @brief Stops a static buffer.
* @param snd The sound associated with the buffer
*/
void Audio_StopStaticBuffer(Sound* snd);

/**
* @brief Returns how many milliseconds a buffer has been playing.
* @param snd The sound associated with the buffer
* @return Elapsed play time in milliseconds
*/
u32 Audio_GetStaticBufferElapsedMs(Sound* snd);
/**
* @brief Sets the play position of static buffer
* @param snd The sound associated with the buffer
* @param milliseconds New play position in milliseconds
*/
void Audio_SetBufferElapsedMs(Sound* snd, u32 milliseconds);

/**
* @brief Returns size of the sound's data in bytes.
* @note For streaming format this is the size needed by the player, not the full size of the song.
* @param snd The sound
* @return Size in bytes
*/
sizetype Audio_GetSoundSizeBytes(Sound* snd);

/**
* @brief Unloads a given sound and frees the memory
* @param snd The sound to free
*/
void Audio_Platform_UnloadSound(Sound* s);


/**
* @brief Starts streaming a sound. The callback function must be set before calling this.
* @param snd The sound to stream
* @param sampleRate The samplerate of the audio.
*/
void Audio_Platform_StartStream(Sound* s, s32 sampleRate);
void Audio_Platform_StopStream(Sound* s);

#ifdef __cplusplus
}
#endif
