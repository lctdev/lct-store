#ifndef FONT_CONSTANTS_H
#define FONT_CONSTANTS_H

#include <foun/foun_primitives.h>

#include <pack/pack_assetHeader.h>

namespace lct
{
namespace font
{
const pack::AssetCode GROUP_CODE = { 'F', 'O', 'N', 'T' };

const pack::AssetCode SHEET_TYPE_CODE = { 'S', 'H', 'E', 'E' };

const u16 VERSION = 0;

const u32 MAX_NAME_SIZE = 32;

const u32 QUAD_VERTEX_COUNT = 4;

const u32 QUAD_INDEX_COUNT = 6;

const u32 TEXTURE_UNIT_INDEX = 0;

enum HorizontalAlign
{
	HORIZONTAL_ALIGN_LEFT = 0,
	HORIZONTAL_ALIGN_MIDDLE
};

enum VerticalAlign
{
	VERTICAL_ALIGN_NONE = 0,
	VERTICAL_ALIGN_TOP,
	VERTICAL_ALIGN_MIDDLE
};

enum Uniform
{
	UNIFORM_WORLD_TRANSFORM = 0,
	UNIFORM_PROJECTION_TRANSFORM,
	UNIFORM_MODULATE_COLOR,
	UNIFORM_TEXTURE,
	UNIFORM_COUNT
};

//namespace font
}
//namespace lct
}

#endif//FONT_CONSTANTS_H
