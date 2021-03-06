#ifndef AUDI_RESOURCES_H
#define AUDI_RESOURCES_H

#include <foun/foun_primitives.h>

#if defined(LCT_WINDOWS)
#include <AL/al.h>
#include <AL/alc.h>
#elif defined(LCT_OSX)
#include <AL/al.h>
#include <AL/alc.h>
#elif defined(LCT_IOS)
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

namespace lct
{
namespace audi
{

struct VoiceResource
{
	ALuint hSource;
	bool used;
};

struct WaveResource
{
	ALuint hBuffer;
};

//namespace audi
}
//namespace lct
}

#endif//AUDI_RESOURCES_H
