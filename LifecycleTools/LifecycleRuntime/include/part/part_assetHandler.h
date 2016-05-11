#ifndef PART_ASSET_HANDLER_H
#define PART_ASSET_HANDLER_H

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

namespace part
{

struct FieldAsset;
struct ParticleControlData;
struct ParticleUniformData;

class AssetHandler : public pack::IAssetHandler
{
public:
	AssetHandler();
	virtual ~AssetHandler();

	void SetAllocator(foun::Allocator* pAllocator);
	void SetAssetContainer(pack::AssetContainer* pAssetContainer);

	virtual bool LoadAsset(pack::AssetHeader& assetHeader, util::BinaryReader& binaryReader);

	FieldAsset* LoadFieldAsset(util::BinaryReader& binaryReader);

	void FixupAllAssets();

private:
	void CopyControlToUniform(ParticleControlData* paParticleControlData, u32 controlCount, f32 globalAngle, ParticleUniformData* pParticleUniformData);

	foun::Allocator* m_pAllocator;
	pack::AssetContainer* m_pAssetContainer;
};

//namespace part
}
//namespace lct
}

#endif//PART_ASSET_HANDLER_H
