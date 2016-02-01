#include <part/part_arrays.h>

#include <grap/grap_data.h>

namespace lct
{
namespace part
{

grap::AttributeData ATTRIBUTE_DATA_ARRAY[ATTRIBUTE_COUNT] =
{
	{ "attr_position", 3 },
	{ "attr_texCoord", 2 },
	{ "attr_color", 4 }
};

grap::UniformData UNIFORM_DATA_ARRAY[UNIFORM_COUNT] =
{
	{ "unif_worldTransform" },
	{ "unif_projectionTransform" },
	{ "unif_texture" }
};

//namespace part
}
//namespace lct
}
