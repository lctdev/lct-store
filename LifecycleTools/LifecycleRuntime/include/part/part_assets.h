#ifndef PART_ASSETS_H
#define PART_ASSETS_H

#include <foun/foun_primitives.h>

#include <pack/pack_assets.h>
#include <pack/pack_assetHeader.h>

namespace lct
{
namespace util
{
class StringIndexMap;
}
namespace imag
{
struct TextureRegionData;
struct TextureAtlasAsset;
}

namespace part
{

struct ParticleControlData;
struct ParticleUniformData;
struct EmitterData;
struct FieldData;

struct FieldAsset : public pack::Asset
{
	static const pack::AssetCode GROUP_CODE;
	static const pack::AssetCode TYPE_CODE;

	FieldData* pFieldData;
	
	struct Emitter
	{
		EmitterData* pEmitterData;
		ParticleControlData* paParticleControlData;
		ParticleUniformData* pParticleUniformData;

		imag::TextureRegionData* pTextureRegionData;
	};
	Emitter* paEmitters;

	imag::TextureAtlasAsset* pTextureAtlasAsset;
};

//namespace part
}
//namespace lct
}

#endif//PART_ASSETS_H
