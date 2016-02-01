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
struct TextureTableAsset;
}

namespace part
{

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
	};
	Emitter* pEmitters;
};

//namespace part
}
//namespace lct
}

#endif//PART_ASSETS_H
