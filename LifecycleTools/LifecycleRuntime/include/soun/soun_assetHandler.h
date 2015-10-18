#ifndef SOUN_ASSET_HANDLER_H
#define SOUN_ASSET_HANDLER_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>

#include <pack/pack_assetHandler.h>

namespace lct
{
namespace audi
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

namespace soun
{

struct WaveAsset;
struct ClipAsset;
struct RampAsset;
struct SequenceAsset;

class AssetHandler : public pack::IAssetHandler
{
public:
	AssetHandler();
	virtual ~AssetHandler();

	void SetAllocator(foun::Allocator* pAllocator);
	void SetAudioDevice(audi::Device* pAudioDevice);
	void SetAssetContainer(pack::AssetContainer* pAssetContainer);

	virtual bool LoadAsset(pack::AssetHeader& assetHeader, util::BinaryReader& binaryReader);

	WaveAsset* LoadWaveAsset(util::BinaryReader& binaryReader);
	ClipAsset* LoadClipAsset(util::BinaryReader& binaryReader);
	RampAsset* LoadRampAsset(util::BinaryReader& binaryReader);
	SequenceAsset* LoadSequenceAsset(util::BinaryReader& binaryReader);

	void FixupAllAssets();

	void AcquireAllAssetResources();
	void ReleaseAllAssetResources();

private:
	foun::Allocator* m_pAllocator;
	audi::Device* m_pAudioDevice;
	pack::AssetContainer* m_pAssetContainer;
};

//namespace soun
}
//namespace lct
}

#endif//SOUN_ASSET_HANDLER_H
