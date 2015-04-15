#ifndef IMAG_ASSET_PROCESSOR_H
#define IMAG_ASSET_PROCESSOR_H

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
struct TextureAsset;
struct TextureTableAsset;
struct TextureData;
struct TextureResource;
class ResourceHandler;

class AssetProcessor : public pack::IAssetHandler
{
public:
	AssetProcessor();
	virtual ~AssetProcessor();

	void SetAllocator(lct::foun::Allocator* pAllocator);
	void SetResourceHandler(ResourceHandler* pResourceHandler);
	void SetAssetContainer(AssetContainer* pAssetContainer);

	virtual bool LoadAsset(pack::AssetHeader& assetHeader, util::BinaryReader& binaryReader);

	TextureAsset* LoadTextureAsset(util::BinaryReader& binaryReader);
	TextureTableAsset* LoadTextureTableAsset(util::BinaryReader& binaryReader);

	void AcquireAllAssetResources();
	void ReleaseAllAssetResources();

private:
	foun::Allocator* m_pAllocator;
	ResourceHandler* m_pResourceHandler;
	AssetContainer* m_pAssetContainer;
};

//namespace imag
}
//namespace lct
}

#endif//IMAG_ASSET_PROCESSOR_H
