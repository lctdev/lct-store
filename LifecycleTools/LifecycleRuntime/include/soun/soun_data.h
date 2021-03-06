#ifndef SOUN_DATA_H
#define SOUN_DATA_H

#include <foun/foun_primitives.h>

#include <soun/soun_constants.h>

namespace lct
{
namespace soun
{

struct WaveData
{
	char name[MAX_NAME_SIZE];	
	u32 channelCount;
	u32 sampleSize;
	u32 sampleLength;
	u32 sampleRate;
	u32 size;
};

struct SegmentData
{
	char waveName[MAX_NAME_SIZE];
};

struct ClipData
{
	char name[MAX_NAME_SIZE];
	f32 propertyArray[CLIP_PROPERTY_TYPE_COUNT];
	bool loop;
	u8 segmentCount;
	u16 padding;
};

struct KeyData
{
	f32 second;
	f32 value;
	f32 slope;
	u32 curveType;
};

struct TrackData
{
	u8 targetPropertyType;
	u8 finishType;
	u16 padding;
	f32 loopSecond;
	u32 keyCount;	
};

struct RampData
{
	char name[MAX_NAME_SIZE];
	u32 trackCount;
};

struct ClipReferenceData
{
	char clipName[MAX_NAME_SIZE];
};

struct RampReferenceData
{
	char rampName[MAX_NAME_SIZE];
};

struct ActionData
{
	f32 second;
	u32 type;
};

struct BeginClipActionData : ActionData
{
	u32 handleIndex;
	u32 clipIndex;
};

struct EndClipActionData : ActionData
{
	u32 handleIndex;
};

struct SetRampActionData : ActionData
{
	u32 handleIndex;
	u32 rampIndex;
};

struct TimelineData
{
	u32 finishType;
	f32 loopSecond;
	u32 actionCount;
};

struct SequenceData
{
	char name[MAX_NAME_SIZE];
	u32 clipHandleCount;
	u32 clipReferenceCount;
	u32 rampReferenceCount;
	u32 timelineCount;
};

//namespace soun
}
//namespace lct
}

#endif//SOUN_DATA_H
