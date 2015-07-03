#ifndef PASS_DATA_H
#define PASS_DATA_H

#include <foun/foun_primitives.h>

#include <pass/pass_constants.h>

namespace lct
{
namespace pass
{

struct StringData
{
	char name[MAX_NAME_SIZE];
	u32 length;
	u32 size;
};

//namespace pass
}
//namespace lct
}

#endif//PASS_DATA_H
