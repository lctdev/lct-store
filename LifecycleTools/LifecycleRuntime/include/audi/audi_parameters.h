#ifndef AUDI_PARAMETERS_H
#define AUDI_PARAMETERS_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace audi
{

struct WaveResource;

struct WaveSetupParameters
{
	WaveResource* pWaveResource;
	void* pWaveBinary;
	u32 size;
	u32 channelCount;
	u32 sampleSize;
	u32 sampleRate;
};

//namespace audi
}
//namespace lct
}

#endif//AUDI_PARAMETERS_H
