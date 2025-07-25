#include <mgdl/mgdl-rocket.h>
#include "base.h"
#include "device.h"
#include "track.h"
#include "sync.h"

#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-music.h>
#ifdef __cplusplus
#include <cmath>
#include <cstdio>
#else
#include <math.h>
#include <stdio.h>
#endif

static Rocket* instance;

#ifndef SYNC_PLAYER
struct sync_cb rocket_callbacks;
#endif

// These are the rocket sync callback functions
void Rocket_Pause_CB( int flag)
{
	if (flag)
    {
		Rocket_Pause(true);
    }
	else
    {
		Rocket_Pause(false);
    }
}

void Rocket_Pause(bool setPaused)
{
    Music_SetPaused(instance->music, setPaused);
    if (setPaused)
    {
        instance->syncState = SyncPause;
    }
    else
    {
        // Start playing when unpaused for the first time
        if (instance->syncState == SyncStop)
        {
            Rocket_Play();
        }
        instance->syncState = SyncPlay;
    }
}

void Rocket_SetRow_CB( int rowIn)
{
    Rocket_SetRow(rowIn);
}

void Rocket_SetRow(int row)
{
    instance->row = (double)row;
    double elapsed = instance->row / instance->rowRate;
    Music_SetElapsedSeconds(instance->music, elapsed);
}

int Rocket_IsPlaying_CB()
{
    // Get the play state of the audio source
    switch(Rocket_GetState())
	{
		case SyncPlay:
			return 1;
			break;
		case SyncPause:
			return 0;
			break;
		case SyncStop:
			return 0;
			break;
		default:
			return 2;
			break;
	}
}

// Used by getters
double Rocket_GetRow()
{
	return instance->row;
}

int Rocket_GetRowInt()
{
    return (int)(floor(instance->row));
}

// Use for effects not tied to tracks
float Rocket_GetTime()
{
    return instance->musicElapsedSeconds;
}

SyncState Rocket_GetState()
{
    return instance->syncState;
}

// For use in the project

bool Rocket_Connect(RocketTrackFormat trackSource, RocketTrackFormat trackDestination,
                 Music* music, float bpm, int rowsPerBeat)
{
    if (music == NULL)
    {
        perror("No music pointer given to RocketSync\n");
        return false;
    }

    struct sync_device* rocket = sync_create_device("sync");
    #ifndef SYNC_PLAYER

    // NOTE 0 means no error == OK
    int connectOk = -1;
    connectOk = sync_tcp_connect(rocket, "localhost", SYNC_DEFAULT_PORT);

    if (connectOk != 0)
    {
        perror("Could not connect to rocket host\n");
        return false;
    }

    rocket_callbacks.pause = Rocket_Pause_CB;
    rocket_callbacks.is_playing = Rocket_IsPlaying_CB;
    rocket_callbacks.set_row = Rocket_SetRow_CB;

    #endif


    Rocket* singleton = _Rocket_GetSingleton();
    singleton->rocket_device = rocket;
    singleton->music = music;
    singleton->bpm = bpm;
    singleton->rowsPerBeat = rowsPerBeat;
    singleton->rowRate = (bpm / 60.0) * (double)rowsPerBeat;
    singleton->jsonFilename = NULL;
    singleton->row = 0;
    singleton->trackSource = trackSource;
    singleton->trackDestination = trackDestination;
    return true;
}

void Rocket_SetJsonFile(const char* filename)
{
#ifdef GEKKO
	instance->jsonFilename = (char*)malloc(sizeof(char) * strlen(filename) + 1);
	strcpy(instance->jsonFilename, filename);
#else
	instance->jsonFilename = filename;
#endif
}

void Rocket_SetBeatsPerMinute(float bpm)
{
    Rocket* singleton = _Rocket_GetSingleton();
    singleton->bpm = bpm;
    singleton->rowRate = (singleton->bpm / 60.0) * (double)singleton->rowsPerBeat;

}

void Rocket_SetRowsPerBeat(int rowsPerBeat)
{
    Rocket* singleton = _Rocket_GetSingleton();
    singleton->rowsPerBeat = rowsPerBeat;
    singleton->rowRate = (singleton->bpm / 60.0) * (double)rowsPerBeat;
}


void Rocket_PlayTracks()
{
    mgdl_assert_print(instance!=NULL, "No RocketSync instance");
    Rocket_Play();
}


// Call this at the start of the frame
void Rocket_UpdateRow()
{
    instance->musicElapsedSeconds = Music_GetElapsedSeconds(instance->music);
	instance->row = instance->musicElapsedSeconds * instance->rowRate;
    #ifndef SYNC_PLAYER
    if (sync_update(instance->rocket_device, Rocket_GetRowInt(), &rocket_callbacks))
    {
        sync_tcp_connect(instance->rocket_device, "localhost", SYNC_DEFAULT_PORT);
    }
    #endif
}


// Call this when closing down
void Rocket_Disconnect()
{
	sync_destroy_device(instance->rocket_device);
    free(instance->_tracks);
    free(instance);
    instance = NULL;
}

// For internal use

Rocket* _Rocket_GetSingleton()
{
    if (instance == NULL)
    {
        instance = (struct Rocket*)malloc(sizeof(struct Rocket));
        instance->_tracks = (ROCKET_TRACK*)malloc(sizeof(ROCKET_TRACK) * ROCKET_TRACK_AMOUNT);
        for (int i = 0; i < ROCKET_TRACK_AMOUNT; i++)
        {
            instance->_tracks[i] = NULL;
        }
        instance->musicElapsedSeconds = 0.0f;
        instance->syncState = SyncStop;
        instance->_trackCount = 0;
    }
    return instance;
}

// Call when the music should start
void Rocket_Play()
{
    mgdl_assert_print(instance->music != NULL, "No music loaded");
    Music_Play(instance->music, false);
    instance->syncState = SyncPlay;
}


ROCKET_TRACK Rocket_AddTrack(const char* trackName)
{
    if (instance == NULL)
    {
        printf("Rocket: No instance\n");
        return NULL;
    }
    if (instance->rocket_device == NULL)
    {
        printf("Rocket: No device\n");
        return NULL;
    }

    ROCKET_TRACK track = NULL;
    switch(instance->trackSource)
    {
        case TrackEditor:
            track = sync_get_track(instance->rocket_device, trackName);
        break;
        case TrackJSON:
            track = sync_get_track_json(trackName, instance->jsonFilename);
        break;
        case TrackCPP:
            // Tracks are in a header
            mgdl_assert_print(false, "If tracks are in a *.cpp file, do not call this function");
            return track;
        break;
    }

    if (track != NULL)
    {
        Rocket_SetToBeSaved(track);
    }
    return track;
}

    // -----------------------------------------------------------
    // Functions for saving tracks
    void Rocket_SaveTrack(ROCKET_TRACK track)
    {
        mgdl_assert_print(track != NULL, "Rocket track was null");
        switch(instance->trackDestination)
        {
            case TrackCPP:
                save_sync(track, MGDL_ROCKET_FILE_H, MGDL_ROCKET_FILE_CPP);
                break;
            case TrackJSON:
                save_sync_json(track, MGDL_ROCKET_FILE_JSON);
                break;
            case TrackEditor:
                // nop : Tracks are not saved to file
                break;
        }
    }

    // Call to write the header files
    void Rocket_StartSaveToHeader()
    {
        switch(instance->trackDestination)
        {
            case TrackCPP:
                start_save_sync(MGDL_ROCKET_FILE_H, MGDL_ROCKET_FILE_CPP);
            break;
            case TrackJSON:
                // TODO Copy from N64 code
            break;
            case TrackEditor:
                // nop
            break;
        }
    }

    void Rocket_EndSaveToHeader()
    {
        switch(instance->trackDestination)
        {
            case TrackCPP:
                end_save_sync(MGDL_ROCKET_FILE_H, MGDL_ROCKET_FILE_CPP);
            break;
            case TrackJSON:
                // TODO Copy from N64 code
            break;
            case TrackEditor:
                // nop
            break;
        }
    }

    void Rocket_SetToBeSaved(ROCKET_TRACK track)
    {
        for (int i = 0; i < instance->_trackCount; i++)
        {
            if (instance->_tracks[i] == track)
            {
                return;
            }
        }
        instance->_tracks[instance->_trackCount] = track;
        instance->_trackCount += 1;
    }


    void Rocket_SaveAllTracks()
    {
        Rocket_StartSaveToHeader();
        for(size_t i = 0; i < instance->_trackCount; i++)
        {
            Rocket_SaveTrack(instance->_tracks[i]);
        }
        Rocket_EndSaveToHeader();
    }

// ----------------------------------------------------------
// Getters
// ----------------------------------------------------------
ROCKET_TRACK Rocket_GetTrack(unsigned short index)
{
    if (index < ROCKET_TRACK_AMOUNT)
    {
        ROCKET_TRACK t = instance->_tracks[index];
        if (t == NULL)
        {
            printf("GetTrack returning null track!\n");
        }
        return t;
    }
    return NULL;
}

unsigned short Rocket_GetTrackIndex(ROCKET_TRACK track)
{
    for (int i = 0; i < instance->_trackCount; i++)
    {
        if (instance->_tracks[i] == track)
        {
            return i;
        }
    }
    return pow(2, 16) -1;
}

float Rocket_Float(ROCKET_TRACK track)
{
    return (float)(sync_get_val(track, instance->row));
}

double Rocket_Double(ROCKET_TRACK track)
{
    return sync_get_val(track, instance->row);
}

int Rocket_Int(ROCKET_TRACK track)
{
    return (int)(floor(sync_get_val(track, instance->row)));
}

bool Rocket_Bool(ROCKET_TRACK track)
{
    return sync_get_val(track, instance->row) > 0.0 ? true : false;
}
