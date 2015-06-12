#ifndef IMAG_ASSETS_H
#define IMAG_ASSETS_H

#include <foun/foun_primitives.h>

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

struct TextureAsset
{
	TextureData* pTextureData;
	void* pTextureBinary;

	grap::TextureResource* pTextureResource;
};

struct TextureTableAsset
{
	TextureTableData* pTextureTableData;
	void* pTextureTableBinary;

	grap::TextureResource* pTextureResourceArray;
};

//namespace spri
}
//namespace lct
}

#endif//IMAG_ASSETS_H
