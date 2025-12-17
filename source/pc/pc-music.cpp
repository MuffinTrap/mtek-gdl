#ifndef GEKKO
#include <mgdl/mgdl-music.h>

#include <mgdl/mgdl-sound.h>
#include <mgdl/mgdl-music.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-openal.h>
#include <mgdl/pc/mgdl-sound-openal.h>

#include <vorbis/vorbisfile.h>

#include <stdio.h>
#include <ostream>
#include <iostream>
#include <cstring>
#include <limits>

// Disabled for now since the whole ogg is loaded into OpenAL
static StreamingAudioData audioData;
static char* pcmBuffer; // Used to transfer data from Ogg file to OpenAL
#if 0

// ////////////////////////////////////////////////
// Ogg Vorbis library callbacks
// ////////////////////////////////////////////////
std::size_t read_ogg_callback(void* destination, std::size_t size, std::size_t numberMembers, void* fileHandle)
{
    StreamingAudioData* audioData = reinterpret_cast<StreamingAudioData*>(fileHandle);

	// bytes = samples * sample size ?
    ALsizei length = size * numberMembers;

	// If near the end, take only what is left
    if(audioData->sizeConsumed + length > audioData->size)
	{
        length = audioData->size - audioData->sizeConsumed;
	}

	// Reopen if file is closed
    if(!audioData->file.is_open())
    {
        audioData->file.open(audioData->filename, std::ios::binary);
        if(!audioData->file.is_open())
        {
            std::cerr << "ERROR: Could not re-open streaming file \"" << audioData->filename << "\"" << std::endl;
            return 0;
        }
    }

    // Prepare to read data
    char* moreData = new char[length];

	// Seek to current playhead position
    audioData->file.clear();
    audioData->file.seekg(audioData->sizeConsumed);

	// Ready bytes into array
    if(!audioData->file.read(&moreData[0],length))
    {
        if(audioData->file.eof())
        {
            audioData->file.clear(); // just clear the error, we will resolve it later
        }
        else if(audioData->file.fail())
        {
            std::cerr << "ERROR: OGG stream has fail bit set " << audioData->filename << std::endl;
            audioData->file.clear();
            return 0;
        }
        else if(audioData->file.bad())
        {
            perror(("ERROR: OGG stream has bad bit set " + audioData->filename).c_str());
            audioData->file.clear();
            return 0;
        }
    }
    audioData->sizeConsumed += length;

	// Copy over to ogg vorbis
    std::memcpy(destination, &moreData[0], length);

    delete[] moreData;

	// Reset again?
    audioData->file.clear();

    return length;
}

// Called when play position is changed.
std::int32_t seek_ogg_callback(void* fileHandle, ogg_int64_t to, std::int32_t type)
{
    StreamingAudioData* audioData = reinterpret_cast<StreamingAudioData*>(fileHandle);

    if(type == SEEK_CUR)
    {
        audioData->sizeConsumed += to;
    }
    else if(type == SEEK_END)
    {
        audioData->sizeConsumed = audioData->size - to;
    }
    else if(type == SEEK_SET)
    {
        audioData->sizeConsumed = to;
    }
    else
        return -1; // what are you trying to do vorbis?

    if(audioData->sizeConsumed < 0)
    {
        audioData->sizeConsumed = 0;
        return -1;
    }
    if(audioData->sizeConsumed > audioData->size)
    {
        audioData->sizeConsumed = audioData->size;
        return -1;
    }

    return 0;
}

long int tell_ogg_callback(void* fileHandle)
{
    StreamingAudioData* audioData = reinterpret_cast<StreamingAudioData*>(fileHandle);
    return audioData->sizeConsumed;
}

int close_ogg_callback(void* fileHandle)
{
    StreamingAudioData* audioData = reinterpret_cast<StreamingAudioData*>(fileHandle);
    audioData->file.close();
    return 0;
}
#endif

// ////////////////////////////////////////////////
// MusicPC class
// ////////////////////////////////////////////////



#if 0
bool LoadAudioDataStreaming ( const char* filename )
{
    std::cout << "Loading Ogg file: " << filename << std::endl;

	audioData.filename = filename;
	audioData.file.open(filename, std::ios::binary);
	if (audioData.file.is_open() == false)
	{
		std::cerr << "OGG Error: could not open file" << filename << std::endl;
		return false;
	}

	// Seek to end and save position as size
	audioData.file.seekg(0, std::ios_base::beg);
	audioData.file.ignore(std::numeric_limits<std::streamsize>::max());
	audioData.size = audioData.file.gcount();

	// Reset
	audioData.file.clear();
	audioData.file.seekg(0, std::ios_base::beg);
	audioData.sizeConsumed = 0;

	if(audioData.file.eof())
    {
        std::cerr << "ERROR: Already reached EOF without loading data" << std::endl;
		audioData.file.close();
        return false;
    }
    else if(audioData.file.fail())
    {
        std::cerr << "ERROR: Fail bit set" << std::endl;
        return false;
    }
    else if(!audioData.file)
    {
        std::cerr << "ERROR: file is false" << std::endl;
        return false;
    }
    return true;
}

bool OpenOggCallbacks()
{
	// Set up callbacks
	ov_callbacks oggCallbacks;
	oggCallbacks.read_func = read_ogg_callback;
	oggCallbacks.seek_func = seek_ogg_callback;
	oggCallbacks.tell_func = tell_ogg_callback;
	oggCallbacks.close_func = close_ogg_callback;

	// Try opening the file
	if (ov_open_callbacks(reinterpret_cast<void*>(&audioData), &audioData.oggVorbisFile, nullptr, -1, oggCallbacks) < 0)
	{
		std::cerr << "OGG Error: could not use ov_open_callbacks" << std::endl;
		audioData.file.close();
		return false;
	}
	return true;
}

#endif

bool OpenOggNoCallbacks()
{
    if (ov_open_callbacks(audioData.filePointer,
        &audioData.oggVorbisFile, nullptr, 0, OV_CALLBACKS_NOCLOSE) < 0)
    {
        Log_Error("OGG Error: could not use ov_open_callbacks");
        return false;
    }
    return true;
}

bool ReadOggProperties()
{
	// Read audio properties
	vorbis_info* vorbisInfo = ov_info(&audioData.oggVorbisFile, -1);
	audioData.channels = vorbisInfo->channels;
	audioData.bitsPerSample = 16; // TODO Can we get this?
	audioData.sampleRate = vorbisInfo->rate;
	audioData.duration = ov_time_total(&audioData.oggVorbisFile, -1);

	// Set format of audio
	if(audioData.channels == 1 && audioData.bitsPerSample == 8)
		audioData.format = AL_FORMAT_MONO8;
	else if(audioData.channels == 1 && audioData.bitsPerSample == 16)
		audioData.format = AL_FORMAT_MONO16;
	else if(audioData.channels == 2 && audioData.bitsPerSample == 8)
		audioData.format = AL_FORMAT_STEREO8;
	else if(audioData.channels == 2 && audioData.bitsPerSample == 16)
		audioData.format = AL_FORMAT_STEREO16;
	else
	{
		Log_ErrorF("ERROR: unrecognised ogg format: %d channels, %d bps\n", audioData.channels, audioData.bitsPerSample);
		audioData.file.close();
		return false;
	}
	return true;
}

void SetALSourceToOrigo(Music* music)
{
	alCall(alSourcef, audioData.source, AL_PITCH, 1);
	alCall(alSourcef, audioData.source, AL_GAIN, 1.0f);
	// Source is at origo and does not move
	alCall(alSource3f, audioData.source, AL_POSITION, 0,0,0);
	alCall(alSource3f, audioData.source, AL_VELOCITY, 0,0,0);
	alCall(alSourcei, audioData.source, AL_LOOPING, music->isLooping ? AL_TRUE : AL_FALSE);
}

bool VerifyALSource()
{
    // Last check
    bool sourceOk = alCall(alIsSource, audioData.source);
    if (sourceOk == false)
    {
        Log_ErrorF("Audiodata did not get valid AL source ID %d\n", audioData.source);
        return false;
    }
    else
    {
        Log_Info("Ogg file loaded ok\n");
    }

	return true;
}

Music* Music_LoadOgg(const char* filename)
{
    Log_InfoF("Loading music from %s\n", filename);

    Music* music = Music_Create();
    if (LoadFileNonStreaming(filename, music))
    {
        music->type = MusicOgg;
        return music;
    }
    else
    {
		Log_ErrorF("Failed to load music %s\n", filename);
        delete music;
        return nullptr;
    }
}
Music* Music_LoadWav(const char* filename)
{
    Log_InfoF("Loading music from %s\n", filename);

    Sound* snd = Sound_Load(filename);
    if (snd)
    {
        Music* music = Music_Create();
        music->type = MusicWav;
        music->wav = snd;

        // music->source = music->wav->source; // TODO
        return music;
    }
    return nullptr;
}

Music* Music_Create(void)
{
    Music* music = (Music*)malloc(sizeof(Music));
    pcmBuffer = nullptr;
    music->isLooping = false;
    return music;
}

bool LoadAudioDataFilePtr ( const char* filename )
{
    audioData.filename = filename;
    audioData.filePointer = nullptr;
    audioData.filePointer = fopen(filename, "rb");
    if (audioData.filePointer == NULL)
    {
		mgdl_assert_printf(false, "Music_Load did not find %s\n", filename);
        return false;
    }
    return true;
}


// Simple version that loads whole ogg into memory
// without streaming
bool LoadFileNonStreaming ( const char* filename, Music* music )
{
    // Clear errors
    alGetError();
    if (LoadAudioDataFilePtr(filename) == false)
    {
        Log_Error("Music_Load failed to open file\n");
        return false;
    }
    if (OpenOggNoCallbacks() == false)
    {
        Log_Error("Music_Load failed to open ogg callbacks\n");
        return false;
    }
    if (ReadOggProperties() == false)
    {
        Log_Error("Music_Load failed to read ogg properties\n");
        return false;
    }

    alCall(alGenBuffers, 1, audioData.buffers);
	alCall(alGenSources, 1, &audioData.source);
    music->source = audioData.source;


    std::int32_t pcmSize = ov_pcm_total(
        &audioData.oggVorbisFile, -1) * audioData.channels * 2;

    pcmBuffer = new char[pcmSize];
    mgdl_assert_print(pcmBuffer != nullptr, "Out of memory");

    bool result = true;
    std::int32_t readSize = ReadOggToPCMBuffer(pcmBuffer, pcmSize);
    if (readSize > 0)
    {
        Log_Info("PCM data buffered\n");
        alCall(alBufferData, audioData.buffers[0], audioData.format, pcmBuffer, readSize, audioData.sampleRate);
        alCall(alSourcei, audioData.source, AL_BUFFER, audioData.buffers[0]);
        // alCall(alSourceQueueBuffers, audioData.source, 1, &buffer);
    }
    else
    {
        result = false;
    }

    result = VerifyALSource();

    // All data is sent to OpenAL, free memory
    delete[] pcmBuffer;
    pcmBuffer = nullptr;

    fclose(audioData.filePointer);

    ov_clear(&audioData.oggVorbisFile);

    SetALSourceToOrigo(music);

    return result;
}

#if 0
// Version that has multiple buffers
bool LoadFileStreaming ( const char* filename )
{
    if (LoadAudioDataStreaming(filename) == false)
    {
        return false;
    }

    if (OpenOggCallbacks() == false)
    {
        return false;
    }

    if (ReadOggProperties() == false)
    {
        return false;
    }

	// Create and setup OpenAL source
	alCall(alGenBuffers, NUM_BUFFERS, &audioData.buffers[0]);

    SetALSourceToOrigo();

	pcmBuffer = new char[BUFFER_SIZE];
	// Start reading data into OpenAL
    CopyToAL();

    if (VerifyALSource() == false)
    {
        return false;
    }
    return true;
}

// Call this after changing play position to
// reload the data into buffers
void Rebuffer()
{
    CopyToAL();
}
#endif

// this reads from vorbis and copies
// the data to OpenAL memory
void CopyToAL()
{
	for(std::uint8_t i = 0; i < NUM_BUFFERS; i++)
	{
		std::int32_t dataSoFar = ReadOggToPCMBuffer(pcmBuffer, BUFFER_SIZE);
		// Send the filled buffer
		alCall(alBufferData, audioData.buffers[i], audioData.format, pcmBuffer, dataSoFar, audioData.sampleRate);
	}
	// All is read, queu all buffers to the source
	alCall(alSourceQueueBuffers, audioData.source, NUM_BUFFERS, &audioData.buffers[0]);
}

void Music_DeleteData(Music* music)
{
	alCall(alDeleteSources, 1, &music->source);
	alCall(alDeleteBuffers, NUM_BUFFERS, &audioData.buffers[0]);
    if (pcmBuffer != nullptr)
    {
        delete pcmBuffer;
    }
    // ov_clear(&audioData.oggVorbisFile); // Already cleared after loading
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
bool Music_Play(Music* music, bool looping)
{
    Music_SetLooping(music, looping);
	alCall(alSourcePlay, music->source);
    return true;
}
#pragma GCC diagnostic pop

void Music_SetPaused(Music* music, bool pause) {
    ALint sourceState;
    alGetSourcei(music->source, AL_SOURCE_STATE, &sourceState);

    if (sourceState == AL_PLAYING && pause) {
        alCall(alSourcePause, music->source);
    }
    else if (sourceState == AL_PAUSED && !pause) {
		alCall(alSourcePlay, music->source);
	}
}

void Music_SetLooping(Music* music, bool looping)
{
    music->isLooping = looping;
    alCall(alSourcei, music->source, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
}

bool Music_GetLooping(Music* music)
{
    return music->isLooping;
}

void Music_Stop(Music* music ) {
	alCall(alSourceStop, music->source);
}

float Music_GetElapsedSeconds(Music* music) {
	ALfloat secOffset;
	alGetSourcef(music->source, AL_SEC_OFFSET, &secOffset);
	return secOffset;
}

void Music_SetElapsedSeconds(Music* music, float elapsed) {
    // Need to set the vorbis position
    // directly since the file is streamed??
    //ov_time_seek(&audioData.oggVorbisFile, elapsed);
    //Rebuffer();
    // Go back to start

	alCall(alSourcef, music->source, AL_SEC_OFFSET,elapsed);
}

SoundStatus Music_GetStatus(Music* music) {
	    // Get the play state of the audio source
    ALint sourceState;
    alGetSourcei(music->source, AL_SOURCE_STATE, &sourceState);

    if (sourceState == AL_PLAYING) {
        return SoundStatus::SoundPlaying;
    } else if (sourceState == AL_PAUSED) {
        return SoundStatus::SoundPaused;
    } else if (sourceState == AL_STOPPED) {
        return SoundStatus::SoundStopped;
    } else if (sourceState == AL_INITIAL) {
        return SoundStatus::SoundInitial;
    }

    return SoundStatus::SoundInitial;
}

// Returns how many bytes read
// -1 Error in reading
std::int32_t ReadOggToPCMBuffer ( char* buffer, std::int32_t bufferSize)
{
    std::int32_t sizeRead = 0;
    std::int32_t result = 0;

    do
    {
        int readSize = READ_SIZE; // Default amount to read
        // Make sure not to read
        // more than fits into buffer
        int sizeLeft = bufferSize - sizeRead;
        if (sizeLeft < readSize)
        {
            readSize = sizeLeft;
        }

        result = ov_read(
            &audioData.oggVorbisFile,
            &buffer[sizeRead],
            readSize,
            0, // 0: Little endiand, 1: Big endian
            sizeof(short),
            1, // 1: signed short, 0: unsigned short
            &audioData.oggCurrentSection);

        if(result == OV_HOLE)
        {
            Log_Error("ERROR: OV_HOLE found in update of buffer ");
            return -1;
        }
        else if(result == OV_EBADLINK)
        {
            Log_Error("ERROR: OV_EBADLINK found in update of buffer ");
            return -1;
        }
        else if(result == OV_EINVAL)
        {
            Log_Error("ERROR: OV_EINVAL found in update of buffer ");
            return -1;
        }
        else if(result == 0)
        {
            std::int32_t seekResult = ov_raw_seek(&audioData.oggVorbisFile, 0);
            if(seekResult == OV_ENOSEEK)
                Log_Error("ERROR: OV_ENOSEEK found when trying to loop");
            else if(seekResult == OV_EINVAL)
                Log_Error("ERROR: OV_EINVAL found when trying to loop");
            else if(seekResult == OV_EREAD)
                Log_Error("ERROR: OV_EREAD found when trying to loop");
            else if(seekResult == OV_EFAULT)
                Log_Error("ERROR: OV_EFAULT found when trying to loop");
            else if(seekResult == OV_EOF)
                Log_Error("ERROR: OV_EOF found when trying to loop");
            else if(seekResult == OV_EBADLINK)
                Log_Error("ERROR: OV_EBADLINK found when trying to loop");

            if(seekResult != 0)
            {
                Log_Error("ERROR: Unknown error in ov_raw_seek");
                return -1;
            }
        }
        else
        {
            sizeRead += result;
        }
    } while(result > 0);

    return sizeRead;
}

void Music_UpdatePlay(Music* music)
{
    if (Music_GetStatus(music) == SoundStopped)
    {
        return;
    }
    // NOP does not do anything on PC, because OpenAL can loop a buffer
}

#if 0
void Music_UpdatePlay(Music* music)
{
	// How far in are we?
	ALint buffersProcessed = 0;
    alCall(alGetSourcei, audioData.source, AL_BUFFERS_PROCESSED, &buffersProcessed);
    if(buffersProcessed <= 0)
    {
        return;
    }

    // Check all buffers
    while(buffersProcessed--)
    {
        ALuint buffer;
        alCall(alSourceUnqueueBuffers, audioData.source, 1, &buffer);

        std::memset(pcmBuffer,0,BUFFER_SIZE);

        ALsizei dataSizeToBuffer = ReadOggToPCMBuffer(pcmBuffer, BUFFER_SIZE);

        if(dataSizeToBuffer > 0)
        {
            alCall(alBufferData, buffer, audioData.format, pcmBuffer, dataSizeToBuffer, audioData.sampleRate);
            alCall(alSourceQueueBuffers, audioData.source, 1, &buffer);
        }

        if(dataSizeToBuffer < BUFFER_SIZE)
        {
            std::cout << "Data missing" << std::endl;
        }

        /* TODO: Does OpenAL do this automatically?
        // Start over if has stopped
        if (isLooping)
		{
			ALint state;
			alCall(alGetSourcei, audioData.source, AL_SOURCE_STATE, &state);
			if(state != AL_PLAYING)
			{
				alCall(alSourceStop, audioData.source);
				alCall(alSourcePlay, audioData.source);
			}
		}
		*/
    }
}
#endif

#endif // not GEKKO
