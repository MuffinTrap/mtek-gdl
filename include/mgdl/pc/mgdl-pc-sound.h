#pragma once
#include <sndfile.h>

#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-sound.h>
#include <mgdl/mgdl-openal.h>

namespace gdl
{
	class SoundPC : public gdl::Sound
	{
	public:
		SoundPC();
		bool LoadFile(const char* filename) override;
		bool LoadBuffer(const u8* buffer, size_t size) override;
		void UnloadData() override;
		void Play(float pitchOffset = 1.0f, float volumePercent = 100.0f) override;
		void SetPaused(bool pause) override;
		void Stop() override;
		float GetElapsedSeconds() override;
		void SetElapsedSeconds(float elapsed) override;
		gdl::SoundStatus GetStatus() override;
		void UpdatePlay() override {};

		virtual ~SoundPC() override;

	private:

		SNDFILE* sndfile;
		ALuint buffer, source;
		s16* data;

		int sample_rate;
	};
}

// Work in progress
#if 0

#include <vorbis/vorbisfile.h>
#include <cstring>
#include <istream>
#include <fstream>
#include <iostream>

namespace gdl
{
	// For ogg playback
	const std::size_t NUM_BUFFERS = 4;
	const ALsizei BUFFER_SIZE = 65536;

	struct StreamingAudioData
	{
		// These buffers are fed to AL in ring sequence
		ALuint buffers[NUM_BUFFERS];

		// File name is kept in case need to open it again
		std::string filename;
		std::ifstream file;

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

	class MusicPC : public gdl::Sound
	{
	public:
		MusicPC();
		bool LoadFile(const char* filename) override;
		bool LoadBuffer(const u8* buffer, size_t size) override;
		void UnloadData() override;
		void Play(float pitchOffset = 1.0f, float volumePercent = 100.0f) override;
		void SetPaused(bool pause) override;
		void Stop() override;
		float GetElapsedSeconds() override;
		void SetElapsedSeconds(float elapsed) override;
		gdl::SoundStatus GetStatus() override;

		void UpdatePlay() override;

		virtual ~MusicPC() override;

	private:
		void CopyToAL();
		void Rebuffer();

		ALuint buffer, source;
		gdl::StreamingAudioData audioData;

		bool looping;
		int sample_rate;
		char* updateBuffer;
	};
}

#endif
