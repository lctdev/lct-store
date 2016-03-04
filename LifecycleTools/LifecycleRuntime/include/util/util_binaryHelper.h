#ifndef UTIL_BINARY_HELPER_H
#define UTIL_BINARY_HELPER_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace util
{

template <class T>
inline bool TestFlag(T flags, u32 index)
{
	return (((1 << index) & flags) != 0);
}

//namespace util
}
//namespace lct
}

#endif//UTIL_BINARY_HELPER_H
