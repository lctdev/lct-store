#ifndef FOUN_MATH_TABLES_H
#define FOUN_MATH_TABLES_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace foun
{

struct SinTable
{
	static const u32 VALUE_COUNT = 1024;
	static const f32 VALUE_ARRAY[VALUE_COUNT];
};

//namespace foun
}
//namespace lct
}

#endif//FOUN_MATH_TABLES_H
