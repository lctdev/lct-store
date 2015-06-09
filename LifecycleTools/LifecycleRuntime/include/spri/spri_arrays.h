#ifndef SPRI_ARRAYS_H
#define SPRI_ARRAYS_H

#include <spri/spri_constants.h>
#include <spri/spri_data.h>

namespace lct
{
namespace grap
{
struct AttributeData;
struct UniformData;
}

namespace spri
{

const u32 ATTRIBUTE_COUNT = 2;
extern grap::AttributeData ATTRIBUTE_DATA_ARRAY[ATTRIBUTE_COUNT];

extern grap::UniformData UNIFORM_DATA_ARRAY[UNIFORM_COUNT];

const u32 QUAD_VERTEX_COUNT = 4;
extern VertexData QUAD_VERTEX_DATA_ARRAY[QUAD_VERTEX_COUNT];

const u32 QUAD_INDEX_COUNT = 6;
extern IndexData QUAD_INDEX_DATA_ARRAY[QUAD_INDEX_COUNT];

//namespace spri
}
//namespace lct
}

#endif//SPRI_DATA_H