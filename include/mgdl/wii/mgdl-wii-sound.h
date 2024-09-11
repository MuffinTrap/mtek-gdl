/*!	\file mgdl-sound.h
 *	\brief Sound module header
 */
#ifndef _GDL_WII_SOUND_H
#define _GDL_WII_SOUND_H

#include <gccore.h>
#include <cstdio>
#include <string>

#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-sound.h>
#include <mgdl/wii/mgdl-wii-config.h>


#if GDL_NO_SOUND == FALSE

namespace gdl {

/*!	\addtogroup soundModule Sound System Module
 *	\brief Functions and classes for loading and playing back sound and music files
 *	@{
 */

//! Sound handling class
/*!
 *	\details Class object for loading and playing back sound effects with pitch control and stereo panning.
 */
class SoundWii : public Sound {

private:
	short	format;
	u16		freq;
	void	*sData;
	int		sSize;
	short	voiceNumber; // ASND voice number

public:

	SoundWii();
	//! Deconstructor.
	virtual ~SoundWii();

	//! Loads a sound file.
	/*!
	 *	\details This function loads a sound file which can then be played with Play() or Play2D().
	 *
	 *	\note This port of the library only supports uncompressed Microsoft WAV sound files with a sample
	 *		format of 8 or 16-bit and a sample frequency of up to 48KHz, Mono or Stereo.
	 *
	 *	\param[in]	*fileName	File name of sound file to load.
	 *
	 *	\returns Non-zero if the sound file was successfully loaded, otherwise an error occured.
	 */
	bool LoadFile(const char* fileName) override;

	// muffintrap: Added for version 0.100.0-muffintrap
	//! Loads a sound file from buffer
	/*!
	 *	\details This function loads a sound file which can then be played with Play() or Play2D().
	 *
	 *	\note This port of the library only supports uncompressed Microsoft WAV sound files with a sample
	 *		format of 8 or 16-bit and a sample frequency of up to 48KHz, Mono or Stereo.
	 *
	 *	\param[in]	buffer		Address of the buffer
	 *	\param[in]	size		Size of the buffer in bytes
	 *
	 *	\returns Non-zero if the sound file was successfully loaded, otherwise an error occured.
	 */
	bool LoadBuffer(const u8* buffer, size_t size) override;


	//! Deletes the sound data stored in the object
	/*!
	 *	\details Deallocates the memory buffer containing the loaded sound data.
	 *
	 *  \note It is not necessary to call this function when the object gets deconstructed as the deconstructor already does
	 *      it for you.
	 */
	void UnloadData() override;


	//! Plays a sound.
	/*!
	 *  \details Plays a sound with pitch control and its own volume level.
	 *
	 *  \note If no sound file is loaded, nothing will be played.
	 *
	 *	\param[in]	pitch	Pitch of sound (1.0 is normal pitch).
	 *	\param[in]	volume	Volume of sound (100 is full volume).
	 */
	void Play(float pitchOffset = 1.0f, float volumePercent = 100.0f) override;


	//! Plays a sound with 2D stereo panning.
	/*!
	 *	\param[in]	pitch	Pitch of sound (1.0 is normal pitch).
	 *	\param[in]	volume	Volume of sound (100 is full volume).
	 *	\param[in]	x		X position of sound (relative to screen).
	 *	\param[in]	y		Y position of sound (this parameter does nothing for now).
	 */
	void Play2D(float pitch, float volume, float x, float y);

	//! Get elapsed playback time in seconds
	/*!
	 *	\details This function returns how long the sound has been playing in seconds
	 *
	 *	\returns Elapsed playback time in seconds
	 */
	float GetElapsedSeconds() override;

	void SetElapsedSeconds(float elapsed) override;

	//! Get if the voice is playing or not
	/*!
	 * \details This function can be used to know if the voice is currently playing
	 *
	 * \returns True if voice is playing, false if not playing
	 */
	gdl::SoundStatus GetStatus() override;

	//! Pauses the playback of the sound
	/*!
	 *	\details This function pauses the sound if it is playing
	 */
	void SetPaused(bool paused) override;

	//! Stops the playback of the sound
	/*!
	 *	\details This function stops the sound if it is playing
	 */
	void Stop() override;

	void UpdatePlay() override;

	// muffintrap: Added for version 0.100.0-muffintrap
	//! Internal sound loading function. Loads a sound from FILE pointer
	/*!
	 *	\details This function is used by other LoadSound functions
	 *
	 *	\note This port of the library only supports uncompressed Microsoft WAV sound files with a sample
	 *		format of 8 or 16-bit and a sample frequency of up to 48KHz, Mono or Stereo.
	 *
	 *	\param[in]	fp		Pointer to opened file
	 *
	 *	\returns Non-zero if the sound file was successfully loaded, otherwise an error occured.
	 */
	bool LoadSound(FILE* fp);
};


//! Sets master sound volumes.
/*!
 *  \details Changes the master volumes of music playback and sound effects.
 *
 *	\note Changing the music volume will affect the currently playing music. Already playing sound effects however
 *		will not be affected but the next sounds will be.
 *
 *	\param[in]	musicVol	Master music volume.
 *	\param[in]	soundVol	Sound effects volume.
 */
void SetMasterVolumes(float musicVol, float soundVol);


//! Sets the music volume.
/*!
 *	\note Changing the music volume will affect the currently playing music on-the-fly.
 *
 *	\param[in]	volume	Music volume.
 */
void SetMusicVolume(float volume);


// muffintrap: Created a class for music
// so that ogg buffer memory handling becomes easier
class MusicWii : public gdl::Sound
{
	public:

		MusicWii();
		bool LoadFile(const char* filename) override;
		bool LoadBuffer(const uint8_t* buffer, size_t size) override;
		void UnloadData() override;
		void Play(float pitchOffset = 1.0f, float volumePercent = 100.0f) override;
		void SetPaused(bool pause) override;
		void Stop() override;
		float GetElapsedSeconds() override;
		void SetElapsedSeconds(float elapsed) override;
		gdl::SoundStatus GetStatus() override;

		void UpdatePlay() override;

		virtual ~MusicWii() override;
	private:
		uint8_t *oggBuffer;
		size_t bufferSize;
		FILE* oggFile;
		std::string fileNameStr;
		char* filenameChar;

}; // Class Music


//!	Plays a music file.
/*!
 *	\details Plays a music file in the background. Unlike the gdl::Sound class which loads the sound data entirely into
 *	    memory, this one streams audio data off of a music file which is ideal for music tracks as they tend to be lengthy
 *      and take up a bit of memory.
 *
 *	\note If the new music cannot be loaded, this function will attempt to play previous music again
 *	\note To resume a stopped or paused music, use this function but specify NULL as the file name.
 *	\note This port of the library only supports mono or stereo OGG Vorbis audio files. Looping OGG files (logg) and mp3s are
 *	    not supported.
 *
 *	\param[in]	fileName	Name of music file to play.
 *	\param[in]	loop		Loop flag (music will be looped if true).
 */
bool PlayMusic(const char* fileName, bool loop);

// muffintrap: Added a new version of PlayMusic 
// to read ogg files from a buffer instead of file
//!	Plays a music file.
/*!
 *	\details Plays a music file in the background. Unlike the gdl::Sound class which loads the sound data entirely into
 *	    memory, this one streams audio data off of a music file which is ideal for music tracks as they tend to be lengthy
 *      and take up a bit of memory.
 *
 *	\note If the new music cannot be loaded, this function will attempt to play previous music again
 *	\note To resume a stopped or paused music, use this function but specify NULL as the file name.
 *	\note This port of the library only supports mono or stereo OGG Vorbis audio files. Looping OGG files (logg) and mp3s are
 *	    not supported.
 *
 *	\param[in]	buffer	Start of the buffer
 *	\param[in]	size	Size of the buffer
 *	\param[in]	loop		Loop flag (music will be looped if true).
 */
bool PlayMusic(u_char* buffer, size_t size, bool loop);

//! Pauses the currently playing music.
/*!
 *  \details Pauses the current playing music. To resume playback, use gdl::PlayMusic() but specify NULL as the file name.
 */
void PauseMusic();


//! Stops the currently playing music.
void StopMusic();

// muffintrap: split PlayMusic into 2 parts. The public functions
// both use this one
//! Loads and plays music file
/*!
 *  \details Always stops the currently playing music
 *
 *	\param[in]	file	Pointer to the file contents
 *	\param[in]	loop	Loop flag (music will be looped if true)
 */
bool PlayMusic(FILE* file, bool loop);

/*! @} */


} // namespace

#endif // NO SOUND


#endif // _GDL_WII_SOUND_H
