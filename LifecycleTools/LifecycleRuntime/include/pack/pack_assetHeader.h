#ifndef PACK_ASSET_HEADER_H
#define PACK_ASSET_HEADER_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace pack
{

struct AssetHeader
{
	struct Code
	{
		union
		{
			char alpha[4];
			u32 numeric;
		};
	};

	Code groupCode;
	Code typeCode;
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
