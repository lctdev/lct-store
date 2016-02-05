#ifndef PART_DATA_H
#define PART_DATA_H

#include <foun/foun_primitives.h>

#include <part/part_constants.h>

namespace lct
{
namespace part
{

struct VertexData
{
	f32 x;
	f32 y;
	f32 z;
	f32 s;
	f32 t;
	f32 r;
	f32 g;
	f32 b;
	f32 a;
};

typedef u16 IndexData;

struct Range
{
	f32 min;
	f32 max;
};

struct Color
{
	f32 r;
	f32 g;
	f32 b;
};

struct ParticleParameterData
{
	f32 initial;
	f32 velocity;
	f32 acceleration;
	Range frameRange;
};

struct EmitterData
{
	f32 x;
	f32 y;
	f32 globalDirX;
	f32 globalDirY;
	Range expelAngleRange;
	Color color0;
	Color color1;
	f32 delayFrames;
	u32 particleCount;
	u32 flags;
	
	ParticleParameterData aParticleParameterData[PARTICLE_PROPERTY_COUNT];
	Range aParticleMultiplierRanges[PARTICLE_PROPERTY_COUNT];
};

struct FieldData
{
	char name[MAX_NAME_SIZE];
	u32 emitterCount;
};

//namespace part
}
//namespace lct
}

#endif//PART_DATA_H
