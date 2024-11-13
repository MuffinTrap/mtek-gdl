#include "mgdl-rocket.h"
#include "sync.h"

#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-sound.h>
#include <cmath>
#include <stdio.h>

static gdl::RocketSync* instance;

#ifndef SYNC_PLAYER
sync_cb rocket_callbacks;
#endif

// These are the rocket sync callback functions
void RocketPause( int flag)
{
    gdl::RocketSync& singleton = gdl::RocketSync::GetSingleton();
	if (flag)
    {
		singleton.Pause(true);
    }
	else
    {
		singleton.Pause(false);
    }
}

void gdl::RocketSync::Pause(bool setPaused)
{
    instance->music->SetPaused(setPaused);
    if (setPaused)
    {
        instance->syncState = SyncPause;
    }
    else
    {
        // Start playing when unpaused for the first time
        if (instance->syncState == SyncStop)
        {
            instance->Play();
        }
        instance->syncState = SyncPlay;
    }
}

void RocketSetRow( int rowIn)
{
    instance->SetRow(rowIn);
}

void gdl::RocketSync::SetRow(int row)
{
    instance->row = (double)row;
    double elapsed = instance->row / instance->rowRate;
    instance->music->SetElapsedSeconds(elapsed);
}

int RocketIsPlaying()
{
    // Get the play state of the audio source
    switch(instance->GetState())
	{
		case gdl::SyncPlay:
			return 1;
			break;
		case gdl::SyncPause:
			return 0;
			break;
		case gdl::SyncStop:
			return 0;
			break;
		default:
			return 2;
			break;
	}
}

// Used by getters
double gdl::RocketSync::GetRow()
{
	return instance->row;
}

int gdl::RocketSync::GetRowInt()
{
    return static_cast<int>(ceil(instance->row));
}

// Use for effects not tied to tracks
float gdl::RocketSync::GetTime()
{
    return instance->musicElapsedSeconds;
}

gdl::SyncState gdl::RocketSync::GetState()
{
    return instance->syncState;
}

// For use in the project

bool gdl::RocketSync::InitRocket(gdl::Sound* soundFile, float bpm, int rowsPerBeat)
{
    if (soundFile == nullptr)
    {
        perror("No sound file give to RocketSync\n");
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
        rocket_callbacks.pause = RocketPause;
        rocket_callbacks.is_playing = RocketIsPlaying;
        rocket_callbacks.set_row = RocketSetRow;
    }
    #endif


    gdl::RocketSync& singleton = gdl::RocketSync::GetSingleton();
    singleton.rocket_device = rocket;
    singleton.music = soundFile;
    singleton.bpm = bpm;
    singleton.rowsPerBeat = rowsPerBeat;
    singleton.rowRate = (bpm / 60.0) * (double)rowsPerBeat;
    return true;
}

void gdl::RocketSync::StartSync()
{
    gdl_assert_print(instance!=nullptr, "No RocketSync instance");
    instance->Play();
}


// Call this at the start of the frame
void gdl::RocketSync::UpdateRow()
{
    instance->musicElapsedSeconds = instance->music->GetElapsedSeconds();
	instance->row = instance->musicElapsedSeconds * instance->rowRate;
    #ifndef SYNC_PLAYER
    if (sync_update(instance->rocket_device, instance->GetRowInt(), &rocket_callbacks))
    {
        sync_tcp_connect(instance->rocket_device, "localhost", SYNC_DEFAULT_PORT);
    }
    #endif
}

sync_device * gdl::RocketSync::GetDevice()
{
    return instance->rocket_device;
}

// Call this when closing down
void gdl::RocketSync::Disconnect()
{
	sync_destroy_device(instance->rocket_device);
    delete instance;
}

// For internal use
gdl::RocketSync::RocketSync()
{
    instance = nullptr;
    music = nullptr;
    bpm = 0;
    rowsPerBeat = 1;
    row = 0;
    rowRate = 0;
    syncState = gdl::SyncState::SyncStop;
    musicElapsedSeconds = 0.0f;
}

gdl::RocketSync & gdl::RocketSync::GetSingleton()
{
    if (instance == nullptr)
    {
        instance = new RocketSync();
    }
    return *instance;
}

// Call when the music should start
void gdl::RocketSync::Play()
{
    gdl_assert_print(instance->music != nullptr, "No music loaded");
    instance->music->Play();
    instance->syncState = SyncPlay;
}


#ifdef SYNC_PLAYER
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"

    // No getting tracks
    // Tracks are static in a header/cpp file in SYNC_PLAYER mode

    void gdl::RocketSync::SaveTrack(ROCKET_TRACK track)
    {
        // NOP
        // Tracks are not saved in SYNC_PLAYER mode
    }

    // Call to write the header files
    void gdl::RocketSync::StartSaveToHeader()
    {
    // NOP
    }

    void gdl::RocketSync::EndSaveToHeader()
    {
        // nop
    }

    void gdl::RocketSync::SetToBeSaved(ROCKET_TRACK track)
    {
        // nop
    }

    void gdl::RocketSync::SaveAllTracks()
    {
        // nop
    }

    #pragma GCC diagnostic pop

#else

    ROCKET_TRACK gdl::RocketSync::GetTrack(const char* trackName, bool save)
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
        if (save)
        {
            SetToBeSaved(track);
        }
        return track;
    }

    // -----------------------------------------------------------
    // Functions for saving tracks
    void gdl::RocketSync::SaveTrack(ROCKET_TRACK track)
    {
        save_sync(track, MGDL_ROCKET_FILE_H, MGDL_ROCKET_FILE_CPP);
    }

    // Call to write the header files
    void gdl::RocketSync::StartSaveToHeader()
    {
        start_save_sync(MGDL_ROCKET_FILE_H, MGDL_ROCKET_FILE_CPP);
    }

    void gdl::RocketSync::EndSaveToHeader()
    {
        end_save_sync(MGDL_ROCKET_FILE_H, MGDL_ROCKET_FILE_CPP);
    }

    void gdl::RocketSync::SetToBeSaved(ROCKET_TRACK track)
    {
        instance->tracks.push_back(track);
    }

    void gdl::RocketSync::SaveAllTracks()
    {
        StartSaveToHeader();
        for(size_t i = 0; i < instance->tracks.size(); i++)
        {
            SaveTrack(instance->tracks[i]);
        }
        EndSaveToHeader();
    }

#endif // SYNC_PLAYER

// ----------------------------------------------------------
// Getters
// ----------------------------------------------------------
float gdl::RocketSync::GetFloat(ROCKET_TRACK track)
{
#ifdef SYNC_PLAYER
    return static_cast<float>(sync_get_val_ref(track, instance->row));
#else
    return static_cast<float>(sync_get_val_ptr(track, instance->row));
#endif
}

double gdl::RocketSync::GetDouble(ROCKET_TRACK track)
{
#ifdef SYNC_PLAYER
    return static_cast<double>(sync_get_val_ref(track, instance->row));
#else
    return static_cast<double>(sync_get_val_ptr(track, instance->row));
#endif
}

int gdl::RocketSync::GetInt(ROCKET_TRACK track)
{
#ifdef SYNC_PLAYER
    return static_cast<int>(sync_get_val_ref(track, instance->row));
#else
    return static_cast<int>(sync_get_val_ptr(track, instance->row));
#endif
}

bool gdl::RocketSync::GetBool(ROCKET_TRACK track)
{
#ifdef SYNC_PLAYER
    return sync_get_val_ref(track, instance->row) > 0.0f ? true : false;
#else
    return sync_get_val_ptr(track, instance->row) > 0.0f ? true : false;
#endif
}
