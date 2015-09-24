#ifndef IMAG_ASSETS_H
#define IMAG_ASSETS_H

#include <foun/foun_primitives.h>

#include <pack/pack_assets.h>
#include <pack/pack_assetHeader.h>

namespace lct
{
namespace grap
{
struct TextureResource;
}

namespace imag
{

struct TextureData;
struct TextureTableData;

struct TextureAsset : public pack::Asset
{
	static const pack::AssetCode GROUP_CODE;
	static const pack::AssetCode TYPE_CODE;

	TextureData* pTextureData;
	void* pTextureBinary;

	grap::TextureResource* pTextureResource;
};

struct TextureTableAsset : public pack::Asset
{
	static const pack::AssetCode GROUP_CODE;
	static const pack::AssetCode TYPE_CODE;

	TextureTableData* pTextureTableData;
	void* pTextureTableBinary;

	grap::TextureResource* pTextureResourceArray;
};

//namespace imag
}
//namespace lct
}

#endif//IMAG_ASSETS_H
