#ifndef FONT_ASSETS_H
#define FONT_ASSETS_H

#include <foun/foun_primitives.h>

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

struct SheetAsset
{
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
