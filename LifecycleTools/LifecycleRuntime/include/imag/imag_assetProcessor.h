#ifndef IMAG_ASSET_PROCESSOR_H
#define IMAG_ASSET_PROCESSOR_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>

#include <pack/pack_assetHandler.h>

namespace lct
{
namespace grap
{
class Device;
}
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

class AssetProcessor : public pack::IAssetHandler
{
public:
	AssetProcessor();
	virtual ~AssetProcessor();

	void SetAllocator(foun::Allocator* pAllocator);
	void SetGraphicsDevice(grap::Device* pGraphicsDevice);
	void SetAssetContainer(AssetContainer* pAssetContainer);

	virtual bool LoadAsset(pack::AssetHeader& assetHeader, util::BinaryReader& binaryReader);

	TextureAsset* LoadTextureAsset(util::BinaryReader& binaryReader);
	TextureTableAsset* LoadTextureTableAsset(util::BinaryReader& binaryReader);

	void AcquireAllAssetResources();
	void ReleaseAllAssetResources();

private:
	foun::Allocator* m_pAllocator;
	grap::Device* m_pGraphicsDevice;
	AssetContainer* m_pAssetContainer;
};

//namespace imag
}
//namespace lct
}

#endif//IMAG_ASSET_PROCESSOR_H
