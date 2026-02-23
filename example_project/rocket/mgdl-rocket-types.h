#pragma once

#include "track.h"
typedef const struct sync_track* ROCKET_TRACK;

#define ROCKET_TRACK_AMOUNT 512
#define ROCKET_NAME_LENGTH 64
#define ROCKET_NO_ERROR 0

struct RocketVariable
{
	char name[ROCKET_NAME_LENGTH];
	float value;
};
typedef struct RocketVariable RocketVariable;
// End CTOY


struct Sound; // Forward declare of mgdl Sound

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
	TrackJSON, /**< The tracks and values are read from JSON file */
	TrackInvalid /**< Nowhere */
};
typedef enum RocketTrackFormat RocketTrackFormat;

