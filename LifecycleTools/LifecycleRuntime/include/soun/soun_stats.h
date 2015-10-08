#ifndef SOUN_STATS_H
#define SOUN_STATS_H

#include <foun/foun_primitives.h>

#include <soun/soun_constants.h>

namespace lct
{
namespace soun
{

struct ClipStats
{
	bool playing;
	u8 padding[3];
	f32 volume;
	f32 pitch;
};

//namespace soun
}
//namespace lct
}

#endif//SOUN_STATS_H
