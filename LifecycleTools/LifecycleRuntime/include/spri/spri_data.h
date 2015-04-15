#ifndef SPRI_DATA_H
#define SPRI_DATA_H

#include <foun/foun_primitives.h>

#include <spri/spri_constants.h>

namespace lct
{
namespace spri
{

struct VertexData
{
	f32 x;
	f32 y;
	f32 z;
	f32 s;
	f32 t;
};

typedef u8 IndexData;

struct Placement
{
	f32 width;
	f32 height;
	f32 x;
	f32 y;
};

struct PartData
{
	char name[MAX_NAME_SIZE];
	Placement placement;
	f32 propertyArray[PART_PROPERTY_TYPE_COUNT];
	s32 jointIndex;
	s32 patternIndex;
};

struct JointData
{
	char name[MAX_NAME_SIZE];
	f32 propertyArray[JOINT_PROPERTY_TYPE_COUNT];
	u32 flags;
	s32 parentIndex;
};

struct PatternData
{
	char name[MAX_NAME_SIZE];
	char textureTableName[MAX_NAME_SIZE];
	f32 propertyArray[PATTERN_PROPERTY_TYPE_COUNT];
};

struct FigureData
{
	char name[MAX_NAME_SIZE];
	u32 partCount;
	u32 jointCount;
	u32 patternCount;
};

struct StripData
{
	char patternName[MAX_NAME_SIZE];
	char textureTableName[MAX_NAME_SIZE];
};

struct SkinData
{
	char name[MAX_NAME_SIZE];
	u32 stripCount;
};

struct KeyData
{
	f32 frame;
	f32 value;
	f32 slope;
	u32 curveType;
};

struct TrackData
{
	char targetName[MAX_NAME_SIZE];
	u8 targetType;
	u8 targetPropertyType;
	u8 finishType;
	u8 padding;
	u32 keyCount;
};

struct AnimationData
{
	char name[MAX_NAME_SIZE];
	u32 trackCount;
};

//namespace spri
}
//namespace lct
}

#endif//SPRI_DATA_H
