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
}
namespace util
{
class BinaryReader;
}
namespace imag
{
class AssetContainer;
}

namespace font
{

class AssetContainer;
struct SheetAsset;

class AssetProcessor : public pack::IAssetHandler
{
public:
	AssetProcessor();
	virtual ~AssetProcessor();

	void SetAllocator(foun::Allocator* pAllocator);
	void SetAssetContainer(AssetContainer* pAssetContainer);

	virtual bool LoadAsset(pack::AssetHeader& assetHeader, util::BinaryReader& binaryReader);

	SheetAsset* LoadSheetAsset(util::BinaryReader& binaryReader);

	void FixupAllAssets(imag::AssetContainer& textureAssetContainer);

private:
	foun::Allocator* m_pAllocator;
	AssetContainer* m_pAssetContainer;
};

//namespace font
}
//namespace lct
}

#endif//FONT_ASSET_PROCESSOR_H
