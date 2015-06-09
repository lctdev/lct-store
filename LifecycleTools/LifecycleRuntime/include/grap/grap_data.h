#ifndef GRAP_DATA_H
#define GRAP_DATA_H

#include <foun/foun_primitives.h>

#include <grap/grap_constants.h>

namespace lct
{
namespace grap
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

//namespace grap
}
//namespace lct
}

#endif//GRAP_DATA_H
