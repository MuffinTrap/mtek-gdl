#pragma once

/**
 * @file mgdl-audio.h
 * @brief Functions to control global audio parameters
 */

extern "C"
{
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
}
