#ifndef PASS_CONSTANTS_H
#define PASS_CONSTANTS_H

#include <foun/foun_primitives.h>

#include <pack/pack_assetHeader.h>

namespace lct
{
namespace pass
{

const pack::AssetCode GROUP_CODE = { 'P', 'A', 'S', 'S' };

const pack::AssetCode STRING_TYPE_CODE = { 'S', 'T', 'R', 'I' };

const u16 VERSION = 0;

const u32 MAX_NAME_SIZE = 32;

//namespace pass
}
//namespace lct
}

#endif//PASS_CONSTANTS_H
