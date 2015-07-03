#ifndef PASS_ASSET_HANDLER_H
#define PASS_ASSET_HANDLER_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>

#include <pack/pack_assetHandler.h>

namespace lct
{
namespace pack
{
struct AssetHeader;
class AssetContainer;
}
namespace util
{
class BinaryReader;
}

namespace pass
{

struct StringAsset;

class AssetHandler : public pack::IAssetHandler
{
public:
	AssetHandler();
	virtual ~AssetHandler();

	void SetAllocator(foun::Allocator* pAllocator);
	void SetAssetContainer(pack::AssetContainer* pAssetContainer);

	virtual bool LoadAsset(pack::AssetHeader& assetHeader, util::BinaryReader& binaryReader);

	StringAsset* LoadStringAsset(util::BinaryReader& binaryReader);

private:
	foun::Allocator* m_pAllocator;
	pack::AssetContainer* m_pAssetContainer;
};

//namespace pass
}
//namespace lct
}

#endif//PASS_ASSET_HANDLER_H
