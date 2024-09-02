#pragma once

#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-sound.h>

#include <sndfile.h>
#include <AL/al.h>
#include <AL/alc.h>

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

		virtual ~SoundPC() override;

	private:

		SNDFILE* sndfile;
		ALuint buffer, source;
		s16* data;

		int sample_rate;
	};
}
