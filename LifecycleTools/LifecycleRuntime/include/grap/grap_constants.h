#ifndef GRAP_CONSTANTS_H
#define GRAP_CONSTANTS_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace grap
{

const u32 MAX_NAME_SIZE = 32;

enum IndexType
{
	INDEX_TYPE_U8 = 0,
	INDEX_TYPE_U16,
	INDEX_TYPE_COUNT
};

enum BlendType
{
	BLEND_TYPE_NONE = 0,
	BLEND_TYPE_ADDITIVE,
	BLEND_TYPE_MULTIPLICATIVE
};

//namespace grap
}
//namespace lct
}

#endif//GRAP_CONSTANTS_H
