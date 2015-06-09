#include <fill/fill_arrays.h>

#include <grap/grap_data.h>

namespace lct
{
namespace fill
{

grap::AttributeData ATTRIBUTE_DATA_ARRAY[ATTRIBUTE_COUNT] =
{
	{ "attr_position", 3 }
};

grap::UniformData UNIFORM_DATA_ARRAY[UNIFORM_COUNT] =
{
	{ "unif_placement" },
	{ "unif_worldTransform" },
	{ "unif_projectionTransform" },
	{ "unif_modulateColor" }
};

VertexData QUAD_VERTEX_DATA_ARRAY[QUAD_VERTEX_COUNT] =
{
	{ -0.5f, -0.5f, 0.0f },
	{ 0.5f, -0.5f, 0.0f },
	{ 0.5f, 0.5f, 0.0f },
	{ -0.5f, 0.5f, 0.0f }
};

IndexData QUAD_INDEX_DATA_ARRAY[QUAD_INDEX_COUNT] =
{
	0,
	1,
	2,
	2,
	3,
	0
};

//namespace fill
}
//namespace lct
}
