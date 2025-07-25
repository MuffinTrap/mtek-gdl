#pragma once

// This class makes using rocket
// easier but a project using it still needs
// to include and compile the rocket source

#include <mgdl/mgdl-types.h>

// Include these only once in your project
#define MGDL_ROCKET_FILE_H "rocket_tracks.h"
#define MGDL_ROCKET_FILE_CPP "rocket_tracks.cpp"
#define MGDL_ROCKET_FILE_JSON "rocket_tracks.json"

// Forward defines of Rocket types
struct sync_device;
struct sync_track;

typedef const struct sync_track* ROCKET_TRACK;

// For CTOY interface
#define ROCKET_TRACK_AMOUNT 512
#define ROCKET_NAME_LENGTH 64
struct RocketVariable
{
	char name[ROCKET_NAME_LENGTH];
	float value;
};
typedef struct RocketVariable RocketVariable;
// End CTOY


struct Music; // Forward declare of mgdl Music

enum SyncState
{
	SyncPlay,
	SyncPause,
	SyncStop
};
typedef enum SyncState SyncState;

// Where do the tracks and values come from and where they are saved
enum RocketTrackFormat
{
	TrackEditor, /**< Connect to rocket editor and get tracks there */
	TrackCPP, /**< The tracks and values are provided as C++ code */
	TrackJSON /**< The tracks and values are read from JSON file */
};
typedef enum RocketTrackFormat RocketTrackFormat;

struct Rocket
{
	struct sync_device *rocket_device;
	struct Music* music;
	double bpm;
	int rowsPerBeat;
	double row;
	double rowRate;
	SyncState syncState;
	float musicElapsedSeconds;

	ROCKET_TRACK* _tracks;
	unsigned short _trackCount;

	// Reading from json file?
	RocketTrackFormat trackSource;
	RocketTrackFormat trackDestination;
	const char *jsonFilename;
};
typedef struct Rocket Rocket;

#ifdef __cplusplus
extern "C" {
#endif

	// Give these functions to the rocket as callbacks
	void Rocket_Pause_CB(int paused);
	void Rocket_SetRow_CB(int row);
	int Rocket_IsPlaying_CB(void);

	// Supply the rocket connection you created and the music
	Rocket* _Rocket_GetSingleton(void);
	bool Rocket_Connect(RocketTrackFormat trackSource, RocketTrackFormat trackDestination, struct Music* music, float bpm, int beatsPerRow);
	void Rocket_SetJsonFile(const char* filename);
	void Rocket_SetBeatsPerMinute(float bpm);
	void Rocket_SetRowsPerBeat(int rowsPerBeat);
	void Rocket_PlayTracks(void);
	void Rocket_UpdateRow(void);
	void Rocket_Disconnect(void); // Disconnects

	ROCKET_TRACK Rocket_AddTrack(const char* trackName);

	ROCKET_TRACK Rocket_GetTrack(unsigned short index);
	unsigned short Rocket_GetTrackIndex(ROCKET_TRACK track);

	void Rocket_StartSaveToHeader(void);
	void Rocket_SaveTrack(ROCKET_TRACK track);
	void Rocket_EndSaveToHeader(void);

	void Rocket_SetToBeSaved(ROCKET_TRACK track);
	void Rocket_SaveAllTracks(void);

	void Rocket_SetRow(int row);
	double Rocket_GetRow(void);
	int Rocket_GetRowInt(void);
	float Rocket_GetTime(void);

	void Rocket_Play(void);
	void Rocket_Pause(bool setPaused);
	SyncState Rocket_GetState(void);

	float Rocket_Float(ROCKET_TRACK track);
	double Rocket_Double(ROCKET_TRACK track);
	int Rocket_Int(ROCKET_TRACK track);
	bool Rocket_Bool(ROCKET_TRACK track);
#ifdef __cplusplus
}
#endif

