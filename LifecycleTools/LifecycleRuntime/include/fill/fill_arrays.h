#ifndef FILL_ARRAYS_H
#define FILL_ARRAYS_H

#include <fill/fill_constants.h>
#include <fill/fill_data.h>

namespace lct
{
namespace grap
{
	struct AttributeData;
	struct UniformData;
}

namespace fill
{

const u32 ATTRIBUTE_COUNT = 1;
extern grap::AttributeData ATTRIBUTE_DATA_ARRAY[ATTRIBUTE_COUNT];

extern grap::UniformData UNIFORM_DATA_ARRAY[UNIFORM_COUNT];

const u32 QUAD_VERTEX_COUNT = 4;
extern VertexData QUAD_VERTEX_DATA_ARRAY[QUAD_VERTEX_COUNT];

const u32 QUAD_INDEX_COUNT = 6;
extern IndexData QUAD_INDEX_DATA_ARRAY[QUAD_INDEX_COUNT];

//namespace fill
}
//namespace lct
}

#endif//FILL_DATA_H