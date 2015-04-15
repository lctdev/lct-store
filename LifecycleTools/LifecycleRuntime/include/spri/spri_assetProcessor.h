#ifndef SPRI_ASSET_PROCESSOR_H
#define SPRI_ASSET_PROCESSOR_H

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

namespace spri
{

class AssetContainer;
struct FigureAsset;
struct AnimationAsset;

class AssetProcessor : public pack::IAssetHandler
{
public:
	AssetProcessor();
	virtual ~AssetProcessor();

	void SetAllocator(foun::Allocator* pAllocator);
	void SetAssetContainer(AssetContainer* pAssetContainer);

	virtual bool LoadAsset(pack::AssetHeader& assetHeader, util::BinaryReader& binaryReader);

	FigureAsset* LoadFigureAsset(util::BinaryReader& binaryReader);
	AnimationAsset* LoadAnimationAsset(util::BinaryReader& binaryReader);

	void FixupAllAssets(imag::AssetContainer& textureAssetContainer);

private:
	foun::Allocator* m_pAllocator;
	AssetContainer* m_pAssetContainer;
};

//namespace spri
}
//namespace lct
}

#endif//SPRI_ASSET_PROCESSOR_H
