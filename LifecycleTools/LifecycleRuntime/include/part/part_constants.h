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

enum ParticleProperty
{
	PARTICLE_PROPERTY_GLOBAL_DISTANCE = 0,
	PARTICLE_PROPERTY_EXPEL_DISTANCE,
	PARTICLE_PROPERTY_SIZE,
	PARTICLE_PROPERTY_ROTATION,
	PARTICLE_PROPERTY_COLOR_RATIO,
	PARTICLE_PROPERTY_ALPHA,
	PARTICLE_PROPERTY_COUNT
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
