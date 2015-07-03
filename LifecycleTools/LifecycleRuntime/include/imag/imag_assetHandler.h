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
class AssetContainer;
}
namespace util
{
class BinaryReader;
}

namespace imag
{

struct TextureAsset;
struct TextureTableAsset;
struct TextureData;

class AssetHandler : public pack::IAssetHandler
{
public:
	AssetHandler();
	virtual ~AssetHandler();

	void SetAllocator(foun::Allocator* pAllocator);
	void SetGraphicsDevice(grap::Device* pGraphicsDevice);
	void SetAssetContainer(pack::AssetContainer* pAssetContainer);

	virtual bool LoadAsset(pack::AssetHeader& assetHeader, util::BinaryReader& binaryReader);

	TextureAsset* LoadTextureAsset(util::BinaryReader& binaryReader);
	TextureTableAsset* LoadTextureTableAsset(util::BinaryReader& binaryReader);

	void AcquireAllAssetResources();
	void ReleaseAllAssetResources();

private:
	foun::Allocator* m_pAllocator;
	grap::Device* m_pGraphicsDevice;
	pack::AssetContainer* m_pAssetContainer;
};

//namespace imag
}
//namespace lct
}

#endif//IMAG_ASSET_PROCESSOR_H
