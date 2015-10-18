#ifndef SOUN_CONSTANTS_H
#define SOUN_CONSTANTS_H

#include <foun/foun_primitives.h>

#include <pack/pack_assetHeader.h>

namespace lct
{
namespace soun
{
const pack::AssetCode GROUP_CODE = { 'S', 'O', 'U', 'N' };

const pack::AssetCode WAVE_TYPE_CODE = { 'W', 'A', 'V', 'E' };
const pack::AssetCode CLIP_TYPE_CODE = { 'C', 'L', 'I', 'P' };
const pack::AssetCode RAMP_TYPE_CODE = { 'R', 'A', 'M', 'P' };
const pack::AssetCode SEQUENCE_TYPE_CODE = { 'S', 'E', 'Q', 'U' };

const u16 VERSION = 0;

const u32 MAX_NAME_SIZE = 32;

enum ClipPropertyType
{
	CLIP_PROPERTY_TYPE_VOLUME = 0,
	CLIP_PROPERTY_TYPE_PITCH,
	CLIP_PROPERTY_TYPE_COUNT
};

enum CurveType
{
	CURVE_TYPE_FLAT = 0,
	CURVE_TYPE_LINEAR,
};

enum ActionType
{
	ACTION_TYPE_NONE = 0,
	ACTION_TYPE_BEGIN_CLIP,
	ACTION_TYPE_END_CLIP,
	ACTION_TYPE_SET_RAMP
};

enum FinishType
{
	FINISH_TYPE_HOLD = 0,
	FINISH_TYPE_LOOP
};

//namespace soun
}
//namespace lct
}

#endif//SOUN_CONSTANTS_H
