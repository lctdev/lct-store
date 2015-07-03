#ifndef FONT_ASSETS_H
#define FONT_ASSETS_H

#include <foun/foun_primitives.h>

#include <pack/pack_assets.h>
#include <pack/pack_assetHeader.h>

namespace lct
{
namespace util
{
class IntegerIndexMap;
}
namespace imag
{
struct TextureAsset;
}

namespace font
{

struct GlyphData;
struct SheetData;

struct SheetAsset : public pack::Asset
{
	static const pack::AssetCode GROUP_CODE;
	static const pack::AssetCode TYPE_CODE;

	SheetData* pSheetData;

	struct Glyph
	{
		GlyphData* pGlyphData;
	};
	Glyph* pGlyphArray;

	imag::TextureAsset* pTextureAsset;

	util::IntegerIndexMap* pGlyphIndexMap;
};

//namespace font
}
//namespace lct
}

#endif//FONT_ASSETS_H
