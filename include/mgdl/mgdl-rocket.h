#pragma once

// This class makes using rocket
// easier but a project using it still needs
// to include and compile the rocket source

// Include these only once in your project
#define MGDL_ROCKET_FILE_H "rocket_tracks.h"
#define MGDL_ROCKET_FILE_CPP "rocket_tracks.cpp"

// Forward defines of Rocket types
struct sync_device;
struct sync_track;

typedef const sync_track* ROCKET_TRACK;

#define ROCKET_TRACK_AMOUNT 512
#define ROCKET_NAME_LENGTH 64
struct RocketVariable
{
	char name[ROCKET_NAME_LENGTH];
	float value;
};

struct Music; // Forward declare of mgdl Music

enum SyncState
{
	SyncPlay,
	SyncPause,
	SyncStop
};

struct Rocket
{
	sync_device *rocket_device;
	Music* music;
	double bpm;
	int rowsPerBeat;
	double row;
	double rowRate;
	SyncState syncState;
	float musicElapsedSeconds;

	ROCKET_TRACK* _tracks;
	unsigned short _trackCount;
};

extern "C" {
	// Give these functions to the rocket as callbacks
	void Rocket_Pause_CB(int paused);
	void Rocket_SetRow_CB(int row);
	int Rocket_IsPlaying_CB();

	// Supply the rocket connection you created and the music
	Rocket* _Rocket_GetSingleton();
	bool Rocket_Init(Music* music, float bpm, int beatsPerRow);
	void Rocket_SetBeatsPerMinute(float bpm);
	void Rocket_SetRowsPerBeat(int rowsPerBeat);
	void Rocket_UpdateRow();
	sync_device* Rocket_GetDevice();
	void Rocket_Disconnect(); // Disconnects
	void Rocket_StartSync();

	ROCKET_TRACK Rocket_AddTrack(const char* trackName);
	ROCKET_TRACK Rocket_AddTempTrack(const char* trackName);

	ROCKET_TRACK Rocket_GetTrack(unsigned short index);
	unsigned short Rocket_GetTrackIndex(ROCKET_TRACK track);

	void Rocket_StartSaveToHeader();
	void Rocket_SaveTrack(ROCKET_TRACK track);
	void Rocket_EndSaveToHeader();

	void Rocket_SetToBeSaved(ROCKET_TRACK track);
	void Rocket_SaveAllTracks();

	void Rocket_SetRow(int row);
	double Rocket_GetRow();
	int Rocket_GetRowInt();
	float Rocket_GetTime();

	void Rocket_Play();
	void Rocket_Pause(bool setPaused);
	SyncState Rocket_GetState();

	float Rocket_Float(ROCKET_TRACK track);
	double Rocket_Double(ROCKET_TRACK track);
	int Rocket_Int(ROCKET_TRACK track);
	bool Rocket_Bool(ROCKET_TRACK track);
}
