#ifndef FONT_CONSTANTS_H
#define FONT_CONSTANTS_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace font
{

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

//namespace font
}
//namespace lct
}

#endif//FONT_CONSTANTS_H
