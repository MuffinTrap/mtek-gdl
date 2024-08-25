#pragma once

#include <rocket/base.h>
#include <rocket/device.h>
#include <rocket/track.h>

#include <vector>

// This class makes using rocket
// easier but a project using it still needs
// to include and compile the rocket source

// Include these only once in your project
#define MGDL_ROCKET_FILE_H "rocket_sync/tracks.h"
#define MGDL_ROCKET_FILE_CPP "rocket_sync/tracks.cpp"

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
		static void InitRocket(sync_device* rocket, gdl::Sound* soundFile, float bpm, int beatsPerRow);
		static void UpdateRow();
		static sync_device* GetDevice();
		static void Disconnect(); // Disconnects

		static void StartSaveToHeader();
		static void SaveTrack(const sync_track* track);
		static void SaveTrack(sync_track& track);
		static void EndSaveToHeader();

		static void SetToBeSaved(const sync_track*);
		static void SaveAllTracks();

		static void SetRow(int row);
		static double GetRow();
		static int GetRowInt();
		static float GetTime();

		void Play();
		void Pause(bool setPaused);
		gdl::SyncState GetState();

		static float GetFloat(sync_track& track);
		static float GetFloat(const sync_track* track);

		static double GetDouble(sync_track& track);
		static double GetDouble(const sync_track* track);

		static int GetInt(sync_track& track);
		static int GetInt(const sync_track* track);

		static bool GetBool(sync_track& track);
		static bool GetBool(const sync_track* track);
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
