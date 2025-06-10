#include <mgdl/mgdl-rocket.h>
#include "base.h"
#include "device.h"
#include "track.h"
#include "sync.h"

#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-music.h>
#include <cmath>
#include <stdio.h>

static Rocket* instance;

#ifndef SYNC_PLAYER
sync_cb rocket_callbacks;
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
    return static_cast<int>(floor(instance->row));
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

bool Rocket_Init(Music* music, float bpm, int rowsPerBeat)
{
    if (music == nullptr)
    {
        perror("No music pointer given to RocketSync\n");
        return false;
    }

    sync_device* rocket = sync_create_device("sync");
    #ifndef SYNC_PLAYER
    if (sync_tcp_connect(rocket, "localhost", SYNC_DEFAULT_PORT))
    {
        perror("Could not connect to rocket host\n");
        return false;
    }
    else
    {
        rocket_callbacks.pause = Rocket_Pause_CB;
        rocket_callbacks.is_playing = Rocket_IsPlaying_CB;
        rocket_callbacks.set_row = Rocket_SetRow_CB;
    }
    #endif


    Rocket* singleton = _Rocket_GetSingleton();
    singleton->rocket_device = rocket;
    singleton->music = music;
    singleton->bpm = bpm;
    singleton->rowsPerBeat = rowsPerBeat;
    singleton->rowRate = (bpm / 60.0) * (double)rowsPerBeat;
    return true;
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


void Rocket_StartSync()
{
    mgdl_assert_print(instance!=nullptr, "No RocketSync instance");
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

sync_device * Rocket_GetDevice()
{
    return instance->rocket_device;
}

// Call this when closing down
void Rocket_Disconnect()
{
	sync_destroy_device(instance->rocket_device);
    delete[] instance->_tracks;
    delete instance;
    instance = nullptr;
}

// For internal use

Rocket* _Rocket_GetSingleton()
{
    if (instance == nullptr)
    {
        instance = new Rocket();
        instance->_tracks = new ROCKET_TRACK[ROCKET_TRACK_AMOUNT];
        instance->musicElapsedSeconds = 0.0f;
        instance->syncState = SyncStop;
        instance->_trackCount = 0;
    }
    return instance;
}

// Call when the music should start
void Rocket_Play()
{
    mgdl_assert_print(instance->music != nullptr, "No music loaded");
    Music_Play(instance->music, false);
    instance->syncState = SyncPlay;
}


#ifdef SYNC_PLAYER
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"

    // No getting tracks
    ROCKET_TRACK Rocket_AddTrack(const char* trackName)
    {
        return nullptr;
    }
    ROCKET_TRACK Rocket_AddTempTrack(const char* trackName)
    {
        return nullptr;
    }
    // Tracks are static in a header/cpp file in SYNC_PLAYER mode

    void Rocket_SaveTrack(ROCKET_TRACK track)
    {
        // NOP
        // Tracks are not saved in SYNC_PLAYER mode
    }

    // Call to write the header files
    void Rocket_StartSaveToHeader()
    {
    // NOP
    }

    void Rocket_EndSaveToHeader()
    {
        // nop
    }

    void Rocket_SetToBeSaved(ROCKET_TRACK track)
    {
        // nop
    }

    void Rocket_SaveAllTracks()
    {
        // nop
    }

    #pragma GCC diagnostic pop

#else

    ROCKET_TRACK Rocket_AddTrack(const char* trackName)
    {
        ROCKET_TRACK track = Rocket_AddTempTrack(trackName);
        if (track != nullptr)
        {
            Rocket_SetToBeSaved(track);
        }
        return track;
    }
    ROCKET_TRACK Rocket_AddTempTrack(const char* trackName)
    {
        if (instance == nullptr)
        {
            printf("No instance\n");
            return nullptr;
        }
        if (instance->rocket_device == nullptr)
        {
            printf("No device\n");
            return nullptr;
        }
        ROCKET_TRACK track = sync_get_track(instance->rocket_device, trackName);
        return track;
    }

    // -----------------------------------------------------------
    // Functions for saving tracks
    void Rocket_SaveTrack(ROCKET_TRACK track)
    {
        mgdl_assert_print(track != nullptr, "Rocket track was null");
        save_sync(track, MGDL_ROCKET_FILE_H, MGDL_ROCKET_FILE_CPP);
    }

    // Call to write the header files
    void Rocket_StartSaveToHeader()
    {
        start_save_sync(MGDL_ROCKET_FILE_H, MGDL_ROCKET_FILE_CPP);
    }

    void Rocket_EndSaveToHeader()
    {
        end_save_sync(MGDL_ROCKET_FILE_H, MGDL_ROCKET_FILE_CPP);
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

#endif // SYNC_PLAYER

// ----------------------------------------------------------
// Getters
// ----------------------------------------------------------
ROCKET_TRACK Rocket_GetTrack(unsigned short index)
{
    if (index < ROCKET_TRACK_AMOUNT)
    {
        return instance->_tracks[index];
    }
    return nullptr;
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
    return static_cast<float>(sync_get_val(track, instance->row));
}

double Rocket_Double(ROCKET_TRACK track)
{
    return sync_get_val(track, instance->row);
}

int Rocket_Int(ROCKET_TRACK track)
{
    return static_cast<int>(floor(sync_get_val(track, instance->row)));
}

bool Rocket_Bool(ROCKET_TRACK track)
{
    return sync_get_val(track, instance->row) > 0.0 ? true : false;
}
