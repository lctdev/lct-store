#ifndef PART_CONSTANTS_H
#define PART_CONSTANTS_H

#include <foun/foun_primitives.h>

#include <pack/pack_assetHeader.h>

namespace lct
{
namespace part
{
const pack::AssetCode GROUP_CODE = { 'P', 'A', 'R', 'T' };

const pack::AssetCode FIELD_TYPE_CODE = { 'F', 'I', 'E', 'L' };

const u16 VERSION = 0;

const u32 MAX_NAME_SIZE = 32;

const u32 QUAD_VERTEX_COUNT = 4;

const u32 QUAD_INDEX_COUNT = 6;

const u32 TEXTURE_UNIT_INDEX = 0;

enum EmitterShapeType
{
	EMITTER_SHAPE_TYPE_POINT = 0,
	EMITTER_SHAPE_TYPE_CIRCLE,
	EMITTER_SHAPE_TYPE_COUNT
};

enum EmitterArrangeType
{
	EMITTER_ARRANGE_TYPE_EVEN_EDGE = 0,
	EMITTER_ARRANGE_TYPE_RANDOM_EDGE,
	EMITTER_ARRANGE_TYPE_RANDOM_FILL,
	EMITTER_ARRANGE_TYPE_COUNT
};

enum EmitterExpelType
{
	EMITTER_EXPEL_TYPE_DEFAULT = 0,
	EMITTER_EXPEL_TYPE_OUTWARD
};

enum ParticlePropertyType
{
	PARTICLE_PROPERTY_TYPE_GLOBAL_DISTANCE = 0,
	PARTICLE_PROPERTY_TYPE_EXPEL_DISTANCE,
	PARTICLE_PROPERTY_TYPE_SIZE,
	PARTICLE_PROPERTY_TYPE_ROTATION,
	PARTICLE_PROPERTY_TYPE_COLOR_RATIO,
	PARTICLE_PROPERTY_TYPE_ALPHA,
	PARTICLE_PROPERTY_TYPE_COUNT
};

enum EmitterFlagType
{
	EMITTER_FLAG_TYPE_REVERSE_ORDER = 0,
	EMITTER_FLAG_TYPE_COUNT
};

enum Uniform
{
	UNIFORM_WORLD_TRANSFORM = 0,
	UNIFORM_PROJECTION_TRANSFORM,
	UNIFORM_TEXTURE,
	UNIFORM_COUNT
};

//namespace part
}
//namespace lct
}

#endif//PART_CONSTANTS_H
