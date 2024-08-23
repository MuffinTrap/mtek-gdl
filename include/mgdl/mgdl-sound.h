#pragma once

#include "mgdl-types.h"
#include <cstdlib>

namespace gdl
{
	enum SoundStatus
	{
		Stopped,
		Playing,
		Paused,
		Unloaded
	};
	// This is an abstract interface for loading and playing sound files of WAV format
	class Sound
	{
	public:
		virtual bool LoadFile(const char* filename) = 0;
		virtual bool LoadBuffer(const u8* buffer, size_t size) = 0;
		virtual void UnloadData() = 0;
		virtual void Play(float pitchOffset = 1.0f, float volumePercent = 100.0f) = 0;
		virtual void SetPaused(bool pause) = 0;
		virtual void Stop() = 0;
		virtual float GetElapsedSeconds() = 0;
		virtual void SetElapsedSeconds(float elapsed) = 0;
		virtual SoundStatus GetStatus() = 0;

		virtual ~Sound() = default;
	};

	// Separate for music?


};
