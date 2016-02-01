#ifndef PART_ARRAYS_H
#define PART_ARRAYS_H

#include <part/part_constants.h>
#include <part/part_data.h>

namespace lct
{
namespace grap
{
struct AttributeData;
struct UniformData;
}

namespace part
{

const u32 ATTRIBUTE_COUNT = 3;
extern grap::AttributeData ATTRIBUTE_DATA_ARRAY[ATTRIBUTE_COUNT];

extern grap::UniformData UNIFORM_DATA_ARRAY[UNIFORM_COUNT];

//namespace part
}
//namespace lct
}

#endif//PART_ARRAYS_H