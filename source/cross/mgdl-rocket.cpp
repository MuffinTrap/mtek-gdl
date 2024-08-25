#include "mgdl/mgdl-rocket.h"
#include "mgdl/mgdl-sound.h"
#include <cmath>

static gdl::RocketSync* instance;

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

void gdl::RocketSync::InitRocket(sync_device* rocket, gdl::Sound* soundFile, float bpm, int rowsPerBeat)
{
   gdl::RocketSync& singleton = gdl::RocketSync::GetSingleton();
   singleton.rocket_device = rocket;
   singleton.music = soundFile;
   singleton.bpm = bpm;
   singleton.rowsPerBeat = rowsPerBeat;
   singleton.rowRate = (bpm / 60.0) * (double)rowsPerBeat;
}

// Call this at the start of the frame
void gdl::RocketSync::UpdateRow()
{
    instance->musicElapsedSeconds = instance->music->GetElapsedSeconds();
	instance->row  = instance->musicElapsedSeconds * instance->rowRate;
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
    instance->music->Play();
}


// -----------------------------------------------------------
// Functions for saving tracks

// Call to write the header files
void gdl::RocketSync::StartSaveToHeader()
{
    start_save_sync(MGDL_ROCKET_FILE_H, MGDL_ROCKET_FILE_CPP);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void gdl::RocketSync::SaveTrack(sync_track& track)
{
    // NOP
    // Tracks are not saved in SYNC_PLAYER mode
}
#pragma GCC diagnostic pop

void gdl::RocketSync::SaveTrack(const sync_track* track)
{
    save_sync(track, MGDL_ROCKET_FILE_H, MGDL_ROCKET_FILE_CPP);
}

void gdl::RocketSync::EndSaveToHeader()
{
    end_save_sync(MGDL_ROCKET_FILE_H, MGDL_ROCKET_FILE_CPP);
}

void gdl::RocketSync::SetToBeSaved(const sync_track* track)
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



// Getters
float gdl::RocketSync::GetFloat(sync_track& track)
{
    return static_cast<float>(sync_get_val_ref(track, instance->row));
}

float gdl::RocketSync::GetFloat(const sync_track* track)
{
    return static_cast<float>(sync_get_val_ptr(track, instance->row));
}

double gdl::RocketSync::GetDouble(sync_track& track)
{
    return sync_get_val_ref(track, instance->row);
}

double gdl::RocketSync::GetDouble(const sync_track* track)
{
    return sync_get_val_ptr(track, instance->row);
}

int gdl::RocketSync::GetInt(sync_track& track)
{
    return static_cast<int>(sync_get_val_ref(track, instance->row));
}

int gdl::RocketSync::GetInt(const sync_track* track)
{
    return static_cast<int>(sync_get_val_ptr(track, instance->row));
}


bool gdl::RocketSync::GetBool(sync_track& track)
{
    return sync_get_val_ref(track, instance->row) > 0.0f ? true : false;
}

bool gdl::RocketSync::GetBool(const sync_track* track)
{
    return sync_get_val_ptr(track, instance->row) > 0.0f ? true : false;
}


