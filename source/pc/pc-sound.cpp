#include <mgdl/pc/mgdl-pc-sound.h>

gdl::SoundPC::SoundPC()
{

}

gdl::SoundPC::~SoundPC()
{
    UnloadData();
}

bool gdl::SoundPC::LoadFile(const char* filename) {
	// Open the WAV file
    SF_INFO sfinfo;
    printf("Opening Music File: %s\n", filename);
    sndfile = sf_open(filename, SFM_READ, &sfinfo);
    if (!sndfile) {
        printf("Error opening the file '%s'\n", filename);
        return false;
    }

    printf("Load audio file. Sample rate %d\n", sfinfo.samplerate);
    sample_rate = sfinfo.samplerate;

    alGenBuffers(1, &buffer);
    alGenSources(1, &source);

    // Read audio data from the WAV file
    ALsizei dataSize = sfinfo.frames * sfinfo.channels * sizeof(short);
    data = (s16*)malloc(dataSize);
    sf_read_short(sndfile, data, sfinfo.frames * sfinfo.channels);

    // Fill OpenAL buffer with audio data
    alBufferData(buffer, AL_FORMAT_STEREO16, data, dataSize, sfinfo.samplerate);

    // Set the source's buffer and play
    alSourcei(source, AL_BUFFER, buffer);

    sf_close(sndfile);
    return true;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
bool gdl::SoundPC::LoadBuffer(const u8* buffer, size_t size) {
	// TODO
	return false;
}
void gdl::SoundPC::Play(float pitchOffset, float volumePercent) {
	alSourcePlay(source);
}

#pragma GCC diagnostic pop

void gdl::SoundPC::UnloadData() {

    if (data != nullptr)
    {
        alDeleteSources(1, &source);
        alDeleteBuffers(1, &buffer);
        free(data);
        data = nullptr;
    }
}


void gdl::SoundPC::SetElapsedSeconds(float elapsed)
{
	alSourcef(source, AL_SEC_OFFSET, elapsed);
}

void gdl::SoundPC::SetPaused(bool pause) {
	if (pause)
    {
    	alSourcePause(source);
    }
	else
    {
    	alSourcePlay(source);
    }
}

void gdl::SoundPC::Stop() {
	alSourceStop(source);
}

float gdl::SoundPC::GetElapsedSeconds() {
	ALint sampleOffset;
	alGetSourcei(source, AL_SAMPLE_OFFSET, &sampleOffset);
	float time_secs = (float)sampleOffset / (float)sample_rate;
	return time_secs;
}

gdl::SoundStatus gdl::SoundPC::GetStatus() {
	    // Get the play state of the audio source
    ALint sourceState;
    alGetSourcei(source, AL_SOURCE_STATE, &sourceState);

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
