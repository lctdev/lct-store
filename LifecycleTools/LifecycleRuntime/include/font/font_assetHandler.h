#ifndef FONT_ASSET_PROCESSOR_H
#define FONT_ASSET_PROCESSOR_H

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

namespace font
{

struct SheetAsset;

class AssetHandler : public pack::IAssetHandler
{
public:
	AssetHandler();
	virtual ~AssetHandler();

	void SetAllocator(foun::Allocator* pAllocator);
	void SetAssetContainer(pack::AssetContainer* pAssetContainer);

	virtual bool LoadAsset(pack::AssetHeader& assetHeader, util::BinaryReader& binaryReader);

	SheetAsset* LoadSheetAsset(util::BinaryReader& binaryReader);

	void FixupAllAssets();

private:
	foun::Allocator* m_pAllocator;
	pack::AssetContainer* m_pAssetContainer;
};

//namespace font
}
//namespace lct
}

#endif//FONT_ASSET_PROCESSOR_H
