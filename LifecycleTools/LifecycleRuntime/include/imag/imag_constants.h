#ifndef IMAG_CONSTANTS_H
#define IMAG_CONSTANTS_H

#include <foun/foun_primitives.h>

#include <pack/pack_assetHeader.h>

namespace lct
{
namespace imag
{
const pack::AssetCode GROUP_CODE = { 'I', 'M', 'A', 'G' };

const pack::AssetCode TEXTURE_TYPE_CODE = { 'T', 'E', 'X', 'T' };
const pack::AssetCode TEXTURE_TABLE_TYPE_CODE = { 'T', 'E', 'T', 'A' };
const pack::AssetCode TEXTURE_ATLAS_TYPE_CODE = { 'T', 'E', 'A', 'T' };

const u16 VERSION = 0;

const u32 MAX_NAME_SIZE = 32;

//namespace imag
}
//namespace lct
}

#endif//IMAG_CONSTANTS_H
