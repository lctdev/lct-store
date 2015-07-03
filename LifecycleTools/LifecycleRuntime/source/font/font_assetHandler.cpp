#include <font/font_assetHandler.h>
#include <font/font_assets.h>
#include <font/font_data.h>

#include <imag/imag_assets.h>
#include <imag/imag_data.h>

#include <pack/pack_assetHeader.h>
#include <pack/pack_assetContainer.h>

#include <util/util_binaryReader.h>
#include <util/util_indexMap.h>

namespace lct
{
namespace font
{

/*
 * Internal Constants
 */

/*
 * Public Instance
 */
AssetHandler::AssetHandler()
: m_pAllocator(NULL)
, m_pAssetContainer(NULL)
{
}

AssetHandler::~AssetHandler()
{
}

void AssetHandler::SetAllocator(lct::foun::Allocator* pAllocator)
{
	m_pAllocator = pAllocator;
}

void AssetHandler::SetAssetContainer(pack::AssetContainer* pAssetContainer)
{
	m_pAssetContainer = pAssetContainer;
}

bool AssetHandler::LoadAsset(pack::AssetHeader& assetHeader, util::BinaryReader& binaryReader)
{
	bool assetLoaded = false;
	if ((assetHeader.groupCode.numeric == GROUP_CODE.numeric) && (assetHeader.version == VERSION))
	{
		if (assetHeader.typeCode.numeric == SHEET_TYPE_CODE.numeric)
		{
			SheetAsset* pSheetAsset = LoadSheetAsset(binaryReader);
			m_pAssetContainer->AddAsset(pSheetAsset, pSheetAsset->pSheetData->name);
			assetLoaded = true;
		}
	}
	return assetLoaded;
}

SheetAsset* AssetHandler::LoadSheetAsset(util::BinaryReader& binaryReader)
{
	SheetData* pSheetData = binaryReader.ReadType<SheetData>();

	SheetAsset::Glyph* pGlyphArray = m_pAllocator->AllocTypeArray<SheetAsset::Glyph>(pSheetData->glyphCount);
	util::IntegerIndexMap* pGlyphIndexMap = m_pAllocator->AllocType<util::IntegerIndexMap>();
	for (u32 glyphIndex = 0; glyphIndex < pSheetData->glyphCount; ++glyphIndex)
	{
		GlyphData* pGlyphData = binaryReader.ReadType<GlyphData>();

		SheetAsset::Glyph* pGlyph = pGlyphArray + glyphIndex;
		pGlyph->pGlyphData = pGlyphData;

		pGlyphIndexMap->AddIndex(pGlyphData->characterCode, glyphIndex, m_pAllocator);
	}

	SheetAsset* pSheetAsset = m_pAllocator->AllocType<SheetAsset>();
	pSheetAsset->pSheetData = pSheetData;
	pSheetAsset->pGlyphArray = pGlyphArray;
	pSheetAsset->pGlyphIndexMap = pGlyphIndexMap;

	return pSheetAsset;
}

void AssetHandler::FixupAllAssets()
{
	for (pack::AssetIterator<SheetAsset> sheetIterator = m_pAssetContainer->GetIterator<SheetAsset>(); !sheetIterator.IsEnd(); sheetIterator.Next())
	{
		SheetAsset* pSheetAsset = sheetIterator.GetAsset();
		SheetData* pSheetData = pSheetAsset->pSheetData;
		imag::TextureAsset* pTextureAsset = m_pAssetContainer->FindAsset<imag::TextureAsset>(pSheetData->textureName);
		pSheetAsset->pTextureAsset = pTextureAsset;
	}
}

//namespace sheet
}
//namespace lct
}
