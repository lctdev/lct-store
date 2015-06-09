#include <spri/spri_arrays.h>

#include <grap/grap_data.h>

namespace lct
{
namespace spri
{

grap::AttributeData ATTRIBUTE_DATA_ARRAY[ATTRIBUTE_COUNT] =
{
	{ "attr_position", 3 },
	{ "attr_texCoord", 2 }
};

grap::UniformData UNIFORM_DATA_ARRAY[UNIFORM_COUNT] =
{
	{ "unif_placement" },
	{ "unif_jointTransform" },
	{ "unif_worldTransform" },
	{ "unif_projectionTransform" },
	{ "unif_modulateColor" },
	{ "unif_interpolateColor" },
	{ "unif_texture" }
};

VertexData QUAD_VERTEX_DATA_ARRAY[QUAD_VERTEX_COUNT] =
{
	{ -0.5f, -0.5f, 0.0f, 0.0f, 0.0f },
	{ 0.5f, -0.5f, 0.0f, 1.0f, 0.0f },
	{ 0.5f, 0.5f, 0.0f, 1.0f, 1.0f },
	{ -0.5f, 0.5f, 0.0f, 0.0f, 1.0f }
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

//namespace spri
}
//namespace lct
}
