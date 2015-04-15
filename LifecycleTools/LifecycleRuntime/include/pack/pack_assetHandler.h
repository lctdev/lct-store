#ifndef PACK_ASSET_HANDLER_H
#define PACK_ASSET_HANDLER_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace util
{
class BinaryReader;
}

namespace pack
{

struct AssetHeader;

class IAssetHandler
{
public:
	virtual ~IAssetHandler() {};

	virtual bool LoadAsset(pack::AssetHeader& assetHeader, util::BinaryReader& binaryReader) = 0;
};

//namespace pack
}
//namespace lct
}

#endif//PACK_ASSET_HANDLER_H
