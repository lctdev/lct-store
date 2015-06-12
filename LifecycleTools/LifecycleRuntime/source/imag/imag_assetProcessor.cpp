#include <imag/imag_assetProcessor.h>
#include <imag/imag_assetContainer.h>
#include <imag/imag_assets.h>
#include <imag/imag_data.h>

#include <grap/grap_device.h>
#include <grap/grap_resources.h>

#include <pack/pack_assetHeader.h>

#include <util/util_binaryReader.h>
#include <util/util_indexMap.h>

#include <string.h>

namespace lct
{
namespace imag
{

/*
 * Internal Constants
 */
static const pack::AssetHeader::Code GROUP_CODE = {'I', 'M', 'A', 'G'};

static const pack::AssetHeader::Code TEXTURE_TYPE_CODE = {'T', 'E', 'X', 'T'};
static const pack::AssetHeader::Code TEXTURE_TABLE_TYPE_CODE = {'T', 'E', 'T', 'A'};

static const u16 VERSION = 0;

/*
 * Public Instance
 */
AssetProcessor::AssetProcessor()
: m_pAllocator(NULL)
, m_pGraphicsDevice(NULL)
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

void AssetProcessor::SetGraphicsDevice(grap::Device* pGraphicsDevice)
{
	m_pGraphicsDevice = pGraphicsDevice;
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
		if (assetHeader.typeCode.numeric == TEXTURE_TYPE_CODE.numeric)
		{
			TextureAsset* pTextureAsset = LoadTextureAsset(binaryReader);
			m_pAssetContainer->AddTextureAsset(pTextureAsset);
			assetLoaded = true;
		}
		else if (assetHeader.typeCode.numeric == TEXTURE_TABLE_TYPE_CODE.numeric)
		{
			TextureTableAsset* pTextureTableAsset = LoadTextureTableAsset(binaryReader);
			m_pAssetContainer->AddTextureTableAsset(pTextureTableAsset);
			assetLoaded = true;
		}
	}
	return assetLoaded;
}

TextureAsset* AssetProcessor::LoadTextureAsset(util::BinaryReader& binaryReader)
{
	TextureData* pTextureData = binaryReader.ReadType<TextureData>();

	void* pTextureBinary = binaryReader.Read(pTextureData->size);

	grap::TextureResource* pTextureResource = m_pAllocator->AllocType<grap::TextureResource>();
	memset(pTextureResource, 0, sizeof(grap::TextureResource));

	TextureAsset* pTextureAsset = m_pAllocator->AllocType<TextureAsset>();
	pTextureAsset->pTextureData = pTextureData;
	pTextureAsset->pTextureBinary = pTextureBinary;
	pTextureAsset->pTextureResource = pTextureResource;

	return pTextureAsset;
}

TextureTableAsset* AssetProcessor::LoadTextureTableAsset(util::BinaryReader& binaryReader)
{
	TextureTableData* pTextureTableData = binaryReader.ReadType<TextureTableData>();

	void* pTextureTableBinary = binaryReader.Read(pTextureTableData->size);

	grap::TextureResource* pTextureResourceArray = m_pAllocator->AllocTypeArray<grap::TextureResource>(pTextureTableData->count);
	memset(pTextureResourceArray, 0, (sizeof(grap::TextureResource) * pTextureTableData->count));

	TextureTableAsset* pTextureTableAsset = m_pAllocator->AllocType<TextureTableAsset>();
	pTextureTableAsset->pTextureTableData = pTextureTableData;
	pTextureTableAsset->pTextureTableBinary = pTextureTableBinary;
	pTextureTableAsset->pTextureResourceArray = pTextureResourceArray;

	return pTextureTableAsset;
}

void AssetProcessor::AcquireAllAssetResources()
{
	for (AssetContainer::TextureIterator textureIterator = m_pAssetContainer->GetTextureIterator(); !textureIterator.IsEnd(); textureIterator.Next())
	{
		TextureAsset* pTextureAsset = textureIterator.GetValue();

		TextureData* pTextureData = pTextureAsset->pTextureData;
		void* pTextureBinary = pTextureAsset->pTextureBinary;
		grap::TextureResource* pTextureResource = pTextureAsset->pTextureResource;

		grap::TextureSetupParameters textureSetupParameters;
		textureSetupParameters.pTextureResource = pTextureResource;
		textureSetupParameters.pTextureBinary = pTextureBinary;
		textureSetupParameters.width = pTextureData->width;
		textureSetupParameters.height = pTextureData->height;
		m_pGraphicsDevice->AcquireTextureResource(textureSetupParameters);
	}

	for (AssetContainer::TextureTableIterator textureTableIterator = m_pAssetContainer->GetTextureTableIterator(); !textureTableIterator.IsEnd(); textureTableIterator.Next())
	{
		TextureTableAsset* pTextureTableAsset = textureTableIterator.GetValue();

		// get the effective values for one texture in the table
		TextureTableData* pTextureTableData = pTextureTableAsset->pTextureTableData;		
		u32 textureWidth = pTextureTableData->width;
		u32 textureHeight = pTextureTableData->height;
		u32 textureSize = textureWidth * textureHeight * sizeof(TexelRGBAData);
		void* pTextureBinary = pTextureTableAsset->pTextureTableBinary;

		for (u32 textureIndex = 0; textureIndex < pTextureTableData->count; ++textureIndex)
		{
			grap::TextureResource* pTextureResource = pTextureTableAsset->pTextureResourceArray + textureIndex;

			grap::TextureSetupParameters textureSetupParameters;
			textureSetupParameters.pTextureResource = pTextureResource;
			textureSetupParameters.pTextureBinary = pTextureBinary;
			textureSetupParameters.width = textureWidth;
			textureSetupParameters.height = textureHeight;
			m_pGraphicsDevice->AcquireTextureResource(textureSetupParameters);

			// advance to the next texture binary in the table
			pTextureBinary = reinterpret_cast<void*>(reinterpret_cast<u32>(pTextureBinary)+textureSize);
		}
	}
}

void AssetProcessor::ReleaseAllAssetResources()
{
	for (AssetContainer::TextureIterator textureIterator = m_pAssetContainer->GetTextureIterator(); !textureIterator.IsEnd(); textureIterator.Next())
	{
		TextureAsset* pTextureAsset = textureIterator.GetValue();

		grap::TextureResource* pTextureResource = pTextureAsset->pTextureResource;

		grap::TextureSetupParameters textureSetupParameters;
		textureSetupParameters.pTextureResource = pTextureResource;
		m_pGraphicsDevice->ReleaseTextureResource(textureSetupParameters);
	}

	for (AssetContainer::TextureTableIterator textureTableIterator = m_pAssetContainer->GetTextureTableIterator(); !textureTableIterator.IsEnd(); textureTableIterator.Next())
	{
		TextureTableAsset* pTextureTableAsset = textureTableIterator.GetValue();

		TextureTableData* pTextureTableData = pTextureTableAsset->pTextureTableData;

		for (u32 textureIndex = 0; textureIndex < pTextureTableData->count; ++textureIndex)
		{
			grap::TextureResource* pTextureResource = pTextureTableAsset->pTextureResourceArray + textureIndex;

			grap::TextureSetupParameters textureSetupParameters;
			textureSetupParameters.pTextureResource = pTextureResource;
			m_pGraphicsDevice->ReleaseTextureResource(textureSetupParameters);
		}
	}
}

//namespace imag
}
//namespace lct
}
