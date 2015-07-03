#ifndef PACK_ASSET_HEADER_H
#define PACK_ASSET_HEADER_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace pack
{

struct AssetCode
{
	union
	{
		char alpha[4];
		u32 numeric;
	};
};

struct AssetHeader
{
	AssetCode groupCode;
	AssetCode typeCode;
	bool bigEndian;
	u8 padding;
	u16 version;
	u32 dataSize;
};

//namespace pack
}
//namespace lct
}

#endif//PACK_ASSET_HEADER_H
