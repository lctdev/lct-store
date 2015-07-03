#ifndef SPRI_ASSET_PROCESSOR_H
#define SPRI_ASSET_PROCESSOR_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>

#include <pack/pack_assetHandler.h>

namespace lct
{
namespace pack
{
class AssetContainer;
struct AssetHeader;
}
namespace util
{
class BinaryReader;
}

namespace spri
{

struct FigureAsset;
struct AnimationAsset;

class AssetHandler : public pack::IAssetHandler
{
public:
	AssetHandler();
	virtual ~AssetHandler();

	void SetAllocator(foun::Allocator* pAllocator);
	void SetAssetContainer(pack::AssetContainer* pAssetContainer);

	virtual bool LoadAsset(pack::AssetHeader& assetHeader, util::BinaryReader& binaryReader);

	FigureAsset* LoadFigureAsset(util::BinaryReader& binaryReader);
	AnimationAsset* LoadAnimationAsset(util::BinaryReader& binaryReader);

	void FixupAllAssets();

private:
	foun::Allocator* m_pAllocator;
	pack::AssetContainer* m_pAssetContainer;
};

//namespace spri
}
//namespace lct
}

#endif//SPRI_ASSET_PROCESSOR_H
