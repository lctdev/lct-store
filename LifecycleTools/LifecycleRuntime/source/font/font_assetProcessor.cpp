#include <font/font_assetProcessor.h>
#include <font/font_assetContainer.h>
#include <font/font_assets.h>
#include <font/font_data.h>

#include <imag/imag_assetContainer.h>
#include <imag/imag_assets.h>
#include <imag/imag_data.h>

#include <pack/pack_assetHeader.h>

#include <util/util_binaryReader.h>
#include <util/util_indexMap.h>

namespace lct
{
namespace font
{

/*
 * Internal Constants
 */
static const pack::AssetHeader::Code GROUP_CODE = {'F', 'O', 'N', 'T'};

static const pack::AssetHeader::Code SHEET_TYPE_CODE = {'S', 'H', 'E', 'E'};

static const u16 VERSION = 0;

/*
 * Public Instance
 */
AssetProcessor::AssetProcessor()
: m_pAllocator(NULL)
, m_pAssetContainer(NULL)
{
}

AssetProcessor::~AssetProcessor()
{
}

void AssetProcessor::SetAllocator(lct::foun::Allocator* pAllocator)
{
	m_pAllocator = pAllocator;
}

void AssetProcessor::SetAssetContainer(AssetContainer* pAssetContainer)
{
	m_pAssetContainer = pAssetContainer;
}

bool AssetProcessor::LoadAsset(pack::AssetHeader& assetHeader, util::BinaryReader& binaryReader)
{
	bool assetLoaded = false;
	if ((assetHeader.groupCode.numeric == GROUP_CODE.numeric) && (assetHeader.version == VERSION))
	{
		if (assetHeader.typeCode.numeric == SHEET_TYPE_CODE.numeric)
		{
			SheetAsset* pSheetAsset = LoadSheetAsset(binaryReader);
			m_pAssetContainer->AddSheetAsset(pSheetAsset);
			assetLoaded = true;
		}
	}
	return assetLoaded;
}

SheetAsset* AssetProcessor::LoadSheetAsset(util::BinaryReader& binaryReader)
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

void AssetProcessor::FixupAllAssets(imag::AssetContainer& textureAssetContainer)
{
	for (AssetContainer::SheetIterator sheetIterator = m_pAssetContainer->GetSheetIterator(); !sheetIterator.IsEnd(); sheetIterator.Next())
	{
		SheetAsset* pSheetAsset = sheetIterator.GetValue();
		SheetData* pSheetData = pSheetAsset->pSheetData;
		imag::TextureAsset* pTextureAsset = textureAssetContainer.FindTextureAsset(pSheetData->textureName);
		pSheetAsset->pTextureAsset = pTextureAsset;
	}
}

//namespace sheet
}
//namespace lct
}
