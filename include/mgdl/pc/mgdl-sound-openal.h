#pragma once
#include <sndfile.h>

#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-sound.h>
#include <mgdl/mgdl-openal.h>


// Work in progress

#include <mgdl/mgdl-music.h>
#include <vorbis/vorbisfile.h>
#include <cstring>
#include <istream>
#include <fstream>
#include <iostream>

	// For ogg playback
	const std::size_t NUM_BUFFERS = 2;
	const ALsizei READ_SIZE = 4096;
	const ALsizei BUFFER_SIZE = READ_SIZE * 4;

	struct StreamingAudioData
	{
		// These buffers are fed to AL in ring sequence
		ALuint buffers[NUM_BUFFERS];

		// File name is kept in case need to open it again
		std::string filename;
		std::ifstream file;
		FILE* filePointer;

		// Information about the audio
		std::uint8_t channels;
		std::int32_t sampleRate;
		std::uint8_t bitsPerSample;

		// Size in bytes
		ALsizei size;

		// OpenAL book keeping information
		ALuint source;
		ALsizei sizeConsumed = 0;
		ALenum format;

		// Identifier for the ogg vorbis library
		OggVorbis_File oggVorbisFile;
		// What part is being played
		int oggCurrentSection = 0;

		// Duration (What unit?)
		std::size_t duration;
	};

	bool LoadFileStreaming(const char* filename);
	bool LoadFileNonStreaming(const char* filename, Music* music);
	void CopyToAL();
	void Rebuffer();
	bool LoadAudioDataStreaming(const char* filename);
	bool LoadAudioDataFilePtr(const char* filename);
	bool OpenOggCallbacks();
	bool OpenOggNoCallbacks();
	bool ReadOggProperties();
	void SetALSourceToOrigo(Music* music);
	bool VerifyALSource();
	std::int32_t ReadOggToPCMBuffer(char* buffer, std::int32_t bufferSize);


	struct SoundOpenAL
	{
		SNDFILE* sndfile;
		ALuint buffer, source;
	}; typedef struct SoundOpenAL SoundOpenAL;
