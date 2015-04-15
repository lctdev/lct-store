#ifndef SHAD_DATA_H
#define SHAD_DATA_H

#include <foun/foun_primitives.h>

#include <shad/shad_constants.h>

namespace lct
{
namespace shad
{

struct AttributeData
{
	char name[MAX_NAME_SIZE];
	u32 componentCount;
};

struct UniformData
{
	char name[MAX_NAME_SIZE];
};

struct ShaderData
{
	u32 vertexShaderSize;
	u32 fragmentShaderSize;
	u32 attributeCount;
};

//namespace shad
}
//namespace lct
}

#endif//SHAD_DATA_H
