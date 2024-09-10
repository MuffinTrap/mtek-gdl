#include <mgdl/pc/mgdl-pc-sound.h>
#include <stdio.h>
#include <ostream>
#include <iostream>
#include <cstring>
#include <limits>



// Wrapper to always error check Open AL calls
#define alCall(function, ...) alCallImpl(__FILE__, __LINE__, function, __VA_ARGS__)
#define alcCall(function, device, ...) alcCallImpl(__FILE__, __LINE__, function, device, __VA_ARGS__)

void check_al_errors(const std::string& filename, const std::uint_fast32_t line)
{
    ALCenum error = alGetError();
    if(error != AL_NO_ERROR)
    {
        std::cerr << "***ERROR*** (" << filename << ": " << line << ")\n" ;
        switch(error)
        {
        case AL_INVALID_NAME:
            std::cerr << "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
            break;
        case AL_INVALID_ENUM:
            std::cerr << "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
            break;
        case AL_INVALID_VALUE:
            std::cerr << "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
            break;
        case AL_INVALID_OPERATION:
            std::cerr << "AL_INVALID_OPERATION: the requested operation is not valid";
            break;
        case AL_OUT_OF_MEMORY:
            std::cerr << "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
            break;
        default:
            std::cerr << "UNKNOWN AL ERROR: " << error;
        }
        std::cerr << std::endl;
    }
}

template<typename alFunction, typename... Params>
auto alCallImpl(const char* filename, const std::uint_fast32_t line, alFunction function, Params... params)
->typename std::enable_if<std::is_same<void,decltype(function(params...))>::value,decltype(function(params...))>::type
{
    function(std::forward<Params>(params)...);
    check_al_errors(filename,line);
}

template<typename alFunction, typename... Params>
auto alCallImpl(const char* filename, const std::uint_fast32_t line, alFunction function, Params... params)
->typename std::enable_if<!std::is_same<void,decltype(function(params...))>::value,decltype(function(params...))>::type
{
    auto ret = function(std::forward<Params>(params)...);
    check_al_errors(filename,line);
    return ret;
}

// ////////////////////////////////////////////////
// Ogg Vorbis library callbacks
// ////////////////////////////////////////////////
std::size_t read_ogg_callback(void* destination, std::size_t size1, std::size_t size2, void* fileHandle)
{
    gdl::StreamingAudioData* audioData = reinterpret_cast<gdl::StreamingAudioData*>(fileHandle);

	// bytes = samples * sample size ?
    ALsizei length = size1 * size2;

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
    gdl::StreamingAudioData* audioData = reinterpret_cast<gdl::StreamingAudioData*>(fileHandle);

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
    gdl::StreamingAudioData* audioData = reinterpret_cast<gdl::StreamingAudioData*>(fileHandle);
    return audioData->sizeConsumed;
}

// ////////////////////////////////////////////////
// MusicPC class
// ////////////////////////////////////////////////

gdl::MusicPC::MusicPC()
{
	updateBuffer = new char[BUFFER_SIZE];
	looping = false;
}
bool gdl::MusicPC::LoadFile(const char* filename)
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

	// Set up callbacks
	ov_callbacks oggCallbacks;
	oggCallbacks.read_func = read_ogg_callback;
	oggCallbacks.seek_func = seek_ogg_callback;
	oggCallbacks.tell_func = tell_ogg_callback;
	oggCallbacks.close_func = nullptr; // TODO Close the file

	// Try opening the file
	if (ov_open_callbacks(reinterpret_cast<void*>(&audioData), &audioData.oggVorbisFile, nullptr, -1, oggCallbacks) < 0)
	{
		std::cerr << "OGG Error: could not use ov_open_callbacks" << std::endl;
		audioData.file.close();
		return false;
	}


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
		std::cerr << "ERROR: unrecognised ogg format: " << audioData.channels << " channels, " << audioData.bitsPerSample << " bps" << std::endl;
		audioData.file.close();
		return false;
	}

	// Create and setup OpenAL source
	alCall(alGenSources, 1, &audioData.source);
	alCall(alSourcef, audioData.source, AL_PITCH, 1);
	alCall(alSourcef, audioData.source, AL_GAIN, 1.0f);
	// Source is at origo and does not move
	alCall(alSource3f, audioData.source, AL_POSITION, 0,0,0);
	alCall(alSource3f, audioData.source, AL_VELOCITY, 0,0,0);
	alCall(alSourcei, audioData.source, AL_LOOPING, AL_FALSE);
	alCall(alGenBuffers, NUM_BUFFERS, &audioData.buffers[0]);

	// Start reading data into OpenAL
    CopyToAL();


    // Last check
    bool sourceOk = alCall(alIsSource, audioData.source);
    if (sourceOk == false)
    {
        std::cerr << "Audiodata did not get valid AL source ID" << audioData.source << std::endl;
        return false;
    }
    else
    {
        std::cout << "Ogg file loaded ok" << std::endl;
    }

	return true;
}

// this reads from vorbis and copies
// the data to OpenAL memory
void gdl::MusicPC::CopyToAL()
{
	for(std::uint8_t i = 0; i < NUM_BUFFERS; i++)
	{
		std::int32_t dataSoFar = 0;
		while (dataSoFar < BUFFER_SIZE)
		{
			std::int32_t result = ov_read(
				&audioData.oggVorbisFile,
				&updateBuffer[dataSoFar],
				BUFFER_SIZE - dataSoFar,
				0,
				2,
				1,
				&audioData.oggCurrentSection);

            if(result == OV_HOLE)
            {
                std::cerr << "ERROR: OV_HOLE found in initial read of buffer " << i << std::endl;
                break;
            }
            else if(result == OV_EBADLINK)
            {
                std::cerr << "ERROR: OV_EBADLINK found in initial read of buffer " << i << std::endl;
                break;
            }
            else if(result == OV_EINVAL)
            {
                std::cerr << "ERROR: OV_EINVAL found in initial read of buffer " << i << std::endl;
                break;
            }
            else if(result == 0)
            {
                std::cerr << "ERROR: EOF found in initial read of buffer " << i << std::endl;
                break;
            }

            dataSoFar += result;
		}
		// Send the filled buffer
		alCall(alBufferData, audioData.buffers[i], audioData.format, updateBuffer, dataSoFar, audioData.sampleRate);
	}
	// All is read, queu all buffers to the source
	alCall(alSourceQueueBuffers, audioData.source, NUM_BUFFERS, &audioData.buffers[0]);
}

// Call this after changing play position to
// reload the data into buffers
void gdl::MusicPC::Rebuffer()
{
    CopyToAL();
}


void gdl::MusicPC::UnloadData()
{
	alCall(alDeleteSources, 1, &audioData.source);
	alCall(alDeleteBuffers, NUM_BUFFERS, &audioData.buffers[0]);
    delete updateBuffer;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void gdl::MusicPC::Play(float pitchOffset, float volumePercent)
{
	alCall(alSourceStop, audioData.source);
	alCall(alSourcePlay, audioData.source);
}
bool gdl::MusicPC::LoadBuffer(const u8* buffer, size_t size)
{
	return false;
}
#pragma GCC diagnostic pop

void gdl::MusicPC::SetPaused(bool pause) {
    ALint sourceState;
    alGetSourcei(audioData.source, AL_SOURCE_STATE, &sourceState);

    if (sourceState == AL_PLAYING && pause) {
        alCall(alSourcePause, audioData.source);
    }
    else if (sourceState == AL_PAUSED && !pause) {
		alCall(alSourcePlay, audioData.source);
	}
}

void gdl::MusicPC::Stop() {
	alCall(alSourceStop, audioData.source);
}

float gdl::MusicPC::GetElapsedSeconds() {
	ALint sampleOffset;
	alGetSourcei(source, AL_SAMPLE_OFFSET, &sampleOffset);
	float time_secs = (float)sampleOffset / (float)audioData.sampleRate;
	return time_secs;
}

void gdl::MusicPC::SetElapsedSeconds(float elapsed) {
    // Need to set the vorbis position
    // directly since the file is streamed
    //ov_time_seek(&audioData.oggVorbisFile, elapsed);
    //Rebuffer();
    // Go back to start
	alCall(alSourcef, audioData.source, AL_SEC_OFFSET,elapsed);
}

gdl::SoundStatus gdl::MusicPC::GetStatus() {
	    // Get the play state of the audio source
    ALint sourceState;
    alGetSourcei(audioData.source, AL_SOURCE_STATE, &sourceState);

    if (sourceState == AL_PLAYING) {
        return gdl::SoundStatus::Playing;
    } else if (sourceState == AL_PAUSED) {
        return gdl::SoundStatus::Paused;
    } else if (sourceState == AL_STOPPED) {
        return gdl::SoundStatus::Stopped;
    } else {
        return gdl::SoundStatus::Unloaded;
    }
}

gdl::MusicPC::~MusicPC()
{
	UnloadData();
}

void gdl::MusicPC::UpdatePlay()
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

        std::memset(updateBuffer,0,BUFFER_SIZE);

        ALsizei dataSizeToBuffer = 0;
        std::int32_t sizeRead = 0;

        while(sizeRead < BUFFER_SIZE)
        {
			// TODO make a shared read function
            std::int32_t result = ov_read(&audioData.oggVorbisFile, &updateBuffer[sizeRead], BUFFER_SIZE - sizeRead, 0, 2, 1, &audioData.oggCurrentSection);
            if(result == OV_HOLE)
            {
                std::cerr << "ERROR: OV_HOLE found in update of buffer " << std::endl;
                break;
            }
            else if(result == OV_EBADLINK)
            {
                std::cerr << "ERROR: OV_EBADLINK found in update of buffer " << std::endl;
                break;
            }
            else if(result == OV_EINVAL)
            {
                std::cerr << "ERROR: OV_EINVAL found in update of buffer " << std::endl;
                break;
            }
            else if(result == 0)
            {
               std::int32_t seekResult = ov_raw_seek(&audioData.oggVorbisFile, 0);
                if(seekResult == OV_ENOSEEK)
                    std::cerr << "ERROR: OV_ENOSEEK found when trying to loop" << std::endl;
                else if(seekResult == OV_EINVAL)
                    std::cerr << "ERROR: OV_EINVAL found when trying to loop" << std::endl;
                else if(seekResult == OV_EREAD)
                    std::cerr << "ERROR: OV_EREAD found when trying to loop" << std::endl;
                else if(seekResult == OV_EFAULT)
                    std::cerr << "ERROR: OV_EFAULT found when trying to loop" << std::endl;
                else if(seekResult == OV_EOF)
                    std::cerr << "ERROR: OV_EOF found when trying to loop" << std::endl;
                else if(seekResult == OV_EBADLINK)
                    std::cerr << "ERROR: OV_EBADLINK found when trying to loop" << std::endl;

                if(seekResult != 0)
                {
                    std::cerr << "ERROR: Unknown error in ov_raw_seek" << std::endl;
                    return;
                }
            }
            sizeRead += result;
        }
        dataSizeToBuffer = sizeRead;

        if(dataSizeToBuffer > 0)
        {
            alCall(alBufferData, buffer, audioData.format, updateBuffer, dataSizeToBuffer, audioData.sampleRate);
            alCall(alSourceQueueBuffers, audioData.source, 1, &buffer);
        }

        if(dataSizeToBuffer < BUFFER_SIZE)
        {
            std::cout << "Data missing" << std::endl;
        }

        // Start over if has stopped
        if (looping)
		{
			ALint state;
			alCall(alGetSourcei, audioData.source, AL_SOURCE_STATE, &state);
			if(state != AL_PLAYING)
			{
				alCall(alSourceStop, audioData.source);
				alCall(alSourcePlay, audioData.source);
			}
		}
    }
}

