#ifndef IMAG_ASSETS_H
#define IMAG_ASSETS_H

#include <foun/foun_primitives.h>

namespace lct
{

namespace imag
{

struct TextureData;
struct TextureTableData;
struct TextureResource;

struct TextureAsset
{
	TextureData* pTextureData;
	void* pTextureBinary;

	TextureResource* pTextureResource;
};

struct TextureTableAsset
{
	TextureTableData* pTextureTableData;
	void* pTextureTableBinary;

	TextureResource* pTextureResourceArray;
};

//namespace spri
}
//namespace lct
}

#endif//IMAG_ASSETS_H
