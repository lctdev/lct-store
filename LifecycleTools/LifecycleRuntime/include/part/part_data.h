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

struct ParticleControlData
{
	u32 propetyType;
	f32 initial;
	f32 velocity;
	f32 acceleration;
	Range velocityFrameRange;
	Range accelerationFrameRange;
};

struct ParticleUniformData
{
	f32 aInitial[PARTICLE_PROPERTY_TYPE_COUNT];
	f32 aVelocity[PARTICLE_PROPERTY_TYPE_COUNT];
	f32 aAcceleration[PARTICLE_PROPERTY_TYPE_COUNT];
	f32 aVelocityFrameMin[PARTICLE_PROPERTY_TYPE_COUNT];
	f32 aVelocityFrameMax[PARTICLE_PROPERTY_TYPE_COUNT];
	f32 aAccelerationFrameMin[PARTICLE_PROPERTY_TYPE_COUNT];
	f32 aAccelerationFrameMax[PARTICLE_PROPERTY_TYPE_COUNT];
	f32 globalDirX;
	f32 globalDirY;
};

struct EmitterData
{	
	f32 x;
	f32 y;	
	f32 shapeSpanA;
	f32 shapeSpanB;
	f32 globalAngle;
	Range expelAngleRange;
	Range expelMultiplierRange;
	Range scaleMultiplierRange;
	Range rotationMultiplierRange;
	Range scaleFluxRange;
	Color color0;
	Color color1;
	f32 delayFrames;
	f32 frameDuration;
	f32 particleFrameLifetime;
	u16 particleCount;
	u16 emitCount;
	u8 shapeType;
	u8 arrangeType;
	u8 flags;
	u8 particleControlCount;
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
