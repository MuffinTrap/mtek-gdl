#pragma once

#include <mgdl/mgdl-sound.h>

struct stb_vorbis;
struct MusicOgg
{
	Sound snd;
	struct stb_vorbis* vorbisfile;
	int lengthSamples;
	float lengthSeconds;
	int sampleRate;
};

#ifdef _cplusplus
extern "C" {
#endif // _cplusplus


#ifdef _cplusplus
}
#endif // _cplusplus
