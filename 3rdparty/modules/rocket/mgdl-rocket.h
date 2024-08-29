#pragma once

#include "base.h"
#include "device.h"
#include "track.h"

#include <vector>

// This class makes using rocket
// easier but a project using it still needs
// to include and compile the rocket source

// Include these only once in your project
#define MGDL_ROCKET_FILE_H "rocket_tracks.h"
#define MGDL_ROCKET_FILE_CPP "rocket_tracks.cpp"

#ifdef SYNC_PLAYER
	typedef const sync_track& ROCKET_TRACK;
#else
	typedef const sync_track* ROCKET_TRACK;
#endif

extern "C" {
	// Give these functions to the rocket as callbacks
	void RocketPause(int paused);
	void RocketSetRow(int row);
	int RocketIsPlaying();
}

namespace gdl
{
	class Sound;

	enum SyncState
	{
		SyncPlay,
		SyncPause,
		SyncStop
	};
	class RocketSync
	{
	public:
		// Supply the rocket connection you created and the music
		static RocketSync& GetSingleton();
		static bool InitRocket(gdl::Sound* soundFile, float bpm, int beatsPerRow);
		static void UpdateRow();
		static sync_device* GetDevice();
		static void Disconnect(); // Disconnects
		static void StartSync();

		// TODO how to do this?
#ifndef SYNC_PLAYER
		static ROCKET_TRACK GetTrack(const char* trackName);
#endif

		static void StartSaveToHeader();
		static void SaveTrack(ROCKET_TRACK track);
		static void EndSaveToHeader();

		static void SetToBeSaved(ROCKET_TRACK track);
		static void SaveAllTracks();

		static void SetRow(int row);
		static double GetRow();
		static int GetRowInt();
		static float GetTime();

		void Play();
		void Pause(bool setPaused);
		gdl::SyncState GetState();

		static float GetFloat(ROCKET_TRACK track);
		static double GetDouble(ROCKET_TRACK track);
		static int GetInt(ROCKET_TRACK track);
		static bool GetBool(ROCKET_TRACK track);
	private:
		RocketSync();
		sync_device *rocket_device;
		gdl::Sound* music;
		double bpm;
		int rowsPerBeat;
		double row;
		double rowRate;
		gdl::SyncState syncState;
		float musicElapsedSeconds;

		std::vector<const sync_track*> tracks;
	};
};
