#ifndef PACK_PACKAGE_WALKER_H
#define PACK_PACKAGE_WALKER_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace util
{
class BinaryReader;
}

namespace pack
{

class IAssetHandler;

class PackageWalker
{
public:
	PackageWalker();

	void AddAssetHandler(IAssetHandler* pAssetHandler);

	void LoadAllAssets(util::BinaryReader& binaryReader);

private:
	static const u32 MAX_ASSET_HANDLER_COUNT = 8;
	IAssetHandler* m_assetHandlerArray[MAX_ASSET_HANDLER_COUNT];
	u32 m_assetHandlerCount;
};

//namespace pack
}
//namespace lct
}

#endif//PACK_PACKAGE_WALKER_H
