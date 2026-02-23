#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-audio.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-alloc.h>

#include "mgdl-rocket.h"
#include "base.h"
#include "device.h"
#include "track.h"
#include "sync.h"

#ifdef __cplusplus
#include <cmath>
#include <cstdio>
#else
#include <math.h>
#include <stdio.h>
#endif

static Rocket* instance;
struct sync_cb rocket_callbacks;

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
    if (setPaused)
    {
        instance->syncState = SyncPause;
        Audio_PauseSound(instance->music);
    }
    else
    {
        mgdlAudioStateEnum state =  Audio_GetSoundStatus(instance->music);
        if (state == mgdlAudioStateEnum::Audio_StateStopped)
        {
            Rocket_Play();
        }
        else if (state == Audio_StatePaused)
        {
            Audio_ResumeSound(instance->music);
        }
        else
        {
            // NOP
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
    Audio_SetSoundElapsedMs(instance->music, elapsed * 1000);
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

RocketTrackFormat Rocket_Connect(RocketTrackFormat trackDestination,
                 Sound* music, float bpm, int rowsPerBeat)
{
    if (music == NULL)
    {
        Log_Error("No music pointer given to RocketSync\n");
        return TrackInvalid;
    }

    struct sync_device* rocket = sync_create_device("sync");
    RocketTrackFormat source = TrackInvalid;

    // Try to connect to editor first
    // NOTE 0 means no error == OK
    int errorCode = -1;
    errorCode = sync_tcp_connect(rocket, "localhost", SYNC_DEFAULT_PORT);

    if (errorCode == ROCKET_NO_ERROR)
    {
        rocket_callbacks.pause = Rocket_Pause_CB;
        rocket_callbacks.is_playing = Rocket_IsPlaying_CB;
        rocket_callbacks.set_row = Rocket_SetRow_CB;
        source = TrackEditor;
    }
    else
    {
        Log_Warning("Could not connect to rocket host\n");
        source = TrackJSON;
    }

    Rocket* singleton = m_Rocket_GetSingleton();
    singleton->rocket_device = rocket;
    singleton->music = music;
    singleton->bpm = bpm;
    singleton->rowsPerBeat = rowsPerBeat;
    singleton->rowRate = (bpm / 60.0) * (double)rowsPerBeat;
    if (singleton->jsonFilename == NULL)
    {
        singleton->jsonFilename = MGDL_ROCKET_FILE_JSON;
    }
    singleton->row = 0;
    singleton->trackSource = source;
    singleton->trackDestination = trackDestination;

    return source;
}

void Rocket_SetJsonFile(const char* filename)
{
    Rocket* singleton = m_Rocket_GetSingleton();
#ifdef GEKKO
	singleton->jsonFilename = (char*)mgdl_AllocateGeneralMemory(sizeof(char) * strlen(filename) + 1);
	strcpy(singleton->jsonFilename, filename);
#else
	singleton->jsonFilename = filename;
#endif
}

void Rocket_SetBeatsPerMinute(float bpm)
{
    Rocket* singleton = m_Rocket_GetSingleton();
    singleton->bpm = bpm;
    singleton->rowRate = (singleton->bpm / 60.0) * (double)singleton->rowsPerBeat;

}

void Rocket_SetRowsPerBeat(int rowsPerBeat)
{
    Rocket* singleton = m_Rocket_GetSingleton();
    singleton->rowsPerBeat = rowsPerBeat;
    singleton->rowRate = (singleton->bpm / 60.0) * (double)rowsPerBeat;
}


void Rocket_PlayTracks()
{

    Rocket* singleton = m_Rocket_GetSingleton();
    mgdl_assert_print(singleton!=NULL, "No RocketSync instance");
    Rocket_Play();
}


// Call this at the start of the frame
void Rocket_UpdateRow()
{
    instance->musicElapsedSeconds = Audio_GetSoundElapsedMs(instance->music) / 1000.0f;
	instance->row = instance->musicElapsedSeconds * instance->rowRate;
    if (instance->trackSource == TrackEditor)
    {
        if (sync_update(instance->rocket_device, Rocket_GetRowInt(), &rocket_callbacks))
        {
            sync_tcp_connect(instance->rocket_device, "localhost", SYNC_DEFAULT_PORT);
        }
    }
}


// Call this when closing down
void Rocket_Disconnect()
{
    Rocket* singleton = m_Rocket_GetSingleton();
	sync_destroy_device(singleton->rocket_device);
    mgdl_FreeGeneralMemory(singleton->_tracks);
    mgdl_FreeGeneralMemory(singleton);
    instance = NULL;
}

// For internal use

Rocket* m_Rocket_GetSingleton()
{
    if (instance == NULL)
    {
        instance = (struct Rocket*)mgdl_AllocateGeneralMemory(sizeof(struct Rocket));
        instance->_tracks = (ROCKET_TRACK*)mgdl_AllocateGeneralMemory(sizeof(ROCKET_TRACK) * ROCKET_TRACK_AMOUNT);
        for (int i = 0; i < ROCKET_TRACK_AMOUNT; i++)
        {
            instance->_tracks[i] = NULL;
        }
        instance->musicElapsedSeconds = 0.0f;
        instance->syncState = SyncStop;
        instance->_trackCount = 0;
        instance->jsonFilename = NULL;
    }
    return instance;
}

// Call when the music should start
void Rocket_Play()
{
    Rocket* singleton = m_Rocket_GetSingleton();
    mgdl_assert_print(singleton->music != NULL, "No music loaded");
    Audio_PlaySound(singleton->music);
    singleton->syncState = SyncPlay;
}


ROCKET_TRACK Rocket_AddTrack(const char* trackName)
{
    Rocket* singleton = m_Rocket_GetSingleton();
    if (singleton == NULL)
    {
        printf("Rocket: No instance\n");
        return NULL;
    }
    if (singleton->rocket_device == NULL)
    {
        printf("Rocket: No device\n");
        return NULL;
    }

    ROCKET_TRACK track = NULL;
    switch(instance->trackSource)
    {
        case TrackEditor:
            track = sync_get_track(singleton->rocket_device, trackName);
        break;
        case TrackJSON:
            track = sync_get_track_json(trackName, singleton->jsonFilename);
        break;
        case TrackCPP:
            // Tracks are in a header
            mgdl_assert_print(false, "If tracks are in a *.cpp file, do not call this function");
        break;
        case TrackInvalid:
            break;
    }

    if (track != NULL)
    {
        Rocket_SetToBeSaved(track);
    }
    return track;
}

    // -----------------------------------------------------------

    void Rocket_SetToBeSaved(ROCKET_TRACK track)
    {

        Rocket* singleton = m_Rocket_GetSingleton();
        for (int i = 0; i < singleton->_trackCount; i++)
        {
            if (singleton->_tracks[i] == track)
            {
                return;
            }
        }
        singleton->_tracks[singleton->_trackCount] = track;
        singleton->_trackCount += 1;
    }


    // Function for saving tracks
    void Rocket_SaveAllTracks()
    {
        Rocket* singleton = m_Rocket_GetSingleton();
        switch(singleton->trackDestination)
        {
            case TrackCPP:
                save_sync_header(singleton->_tracks, singleton->_trackCount, MGDL_ROCKET_FILE_H, MGDL_ROCKET_FILE_CPP);
                break;
            case TrackJSON:
                save_sync_json(singleton->_tracks, singleton->_trackCount, singleton->jsonFilename);
                break;
            case TrackEditor:
                break;
            case TrackInvalid:
                break;
        }
    }

// ----------------------------------------------------------
// Getters
// ----------------------------------------------------------
ROCKET_TRACK Rocket_GetTrack(unsigned short index)
{
    if (index < ROCKET_TRACK_AMOUNT)
    {
        Rocket* singleton = m_Rocket_GetSingleton();
        ROCKET_TRACK t = singleton->_tracks[index];
        if (t == NULL)
        {
            printf("GetTrack returning null track!\n");
        }
        return t;
    }
    return NULL;
}
unsigned short Rocket_GetTrackAmount()
{
    Rocket* singleton = m_Rocket_GetSingleton();
    return singleton->_trackCount;
}

unsigned short Rocket_GetTrackIndex(ROCKET_TRACK track)
{
    Rocket* singleton = m_Rocket_GetSingleton();
    for (int i = 0; i < singleton->_trackCount; i++)
    {
        if (singleton->_tracks[i] == track)
        {
            return i;
        }
    }
    return pow(2, 16) -1;
}

float Rocket_Float(ROCKET_TRACK track)
{
    Rocket* singleton = m_Rocket_GetSingleton();
    return (float)(sync_get_val(track, singleton->row));
}

double Rocket_Double(ROCKET_TRACK track)
{
    Rocket* singleton = m_Rocket_GetSingleton();
    return sync_get_val(track, singleton->row);
}

int Rocket_Int(ROCKET_TRACK track)
{
    Rocket* singleton = m_Rocket_GetSingleton();
    return (int)(floor(sync_get_val(track, singleton->row)));
}

bool Rocket_Bool(ROCKET_TRACK track)
{
    Rocket* singleton = m_Rocket_GetSingleton();
    return sync_get_val(track, singleton->row) > 0.0 ? true : false;
}
