#include <mgdl/pc/mgdl-pc-sound.h>
#include <mgdl/mgdl-assert.h>

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
    printf("Loading sound file: %s\n", filename);
    sndfile = sf_open(filename, SFM_READ, &sfinfo);
    if (!sndfile) {
        printf("Error opening the file '%s'\n", filename);
        return false;
    }

    alCall(alGenBuffers,1, &buffer);
    alCall(alGenSources,1, &source);

    // Read audio data from the WAV file
    ALsizei dataSize = sfinfo.frames * sfinfo.channels * sizeof(s16);
    ALuint format = AL_FORMAT_STEREO16;
    if (sfinfo.channels == 1)
    {
        format = AL_FORMAT_MONO16;
    }
    data = (ALvoid*)malloc(dataSize);
    gdl_assert_print(data != nullptr, "Out of memory!");
    sf_read_raw(sndfile, data, dataSize);

    // Fill OpenAL buffer with audio data
    alCall(alBufferData, buffer, format, data, dataSize, sfinfo.samplerate);

    // Set the source's buffer
    alCall(alSourcei, source, AL_BUFFER, buffer);

    free(data);
    sf_close(sndfile);
    printf("Sound loaded\n");
    return true;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
bool gdl::SoundPC::LoadBuffer(const u8* buffer, size_t size) {
    gdl_assert_print(false, "Not implemented");
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

void gdl::SoundPC::SetLooping(bool looping)
{
    isLooping = looping;
    alSourcei(source, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
}

void gdl::SoundPC::Stop() {
	alSourceStop(source);
}

float gdl::SoundPC::GetElapsedSeconds() {
	ALfloat secOffset;
	alGetSourcef(source, AL_SEC_OFFSET, &secOffset);
	return secOffset;
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
    } else if (sourceState == AL_INITIAL) {
        return gdl::SoundStatus::Initial;
    }
    return gdl::SoundStatus::Initial;
}
