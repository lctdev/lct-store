#include <imag/imag_assetProcessor.h>
#include <imag/imag_assetContainer.h>
#include <imag/imag_assets.h>
#include <imag/imag_resourceHandler.h>
#include <imag/imag_resources.h>
#include <imag/imag_data.h>

#include <pack/pack_assetHeader.h>

#include <util/util_binaryReader.h>
#include <util/util_indexMap.h>

#include <foun/foun_graphicsDebug.h>

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
, m_pResourceHandler(NULL)
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

void AssetProcessor::SetResourceHandler(ResourceHandler* pResourceHandler)
{
	m_pResourceHandler = pResourceHandler;
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

	lct::imag::TextureResource* pTextureResource = m_pAllocator->AllocType<lct::imag::TextureResource>();
	memset(pTextureResource, 0, sizeof(lct::imag::TextureResource));

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

	lct::imag::TextureResource* pTextureResourceArray = m_pAllocator->AllocTypeArray<lct::imag::TextureResource>(pTextureTableData->count);
	memset(pTextureResourceArray, 0, (sizeof(lct::imag::TextureResource) * pTextureTableData->count));

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
		TextureResource* pTextureResource = pTextureAsset->pTextureResource;
		m_pResourceHandler->AcquireTextureResource(pTextureResource, pTextureData->width, pTextureData->height, pTextureBinary);
	}

	for (AssetContainer::TextureTableIterator textureTableIterator = m_pAssetContainer->GetTextureTableIterator(); !textureTableIterator.IsEnd(); textureTableIterator.Next())
	{
		TextureTableAsset* pTextureTableAsset = textureTableIterator.GetValue();

		TextureTableData* pTextureTableData = pTextureTableAsset->pTextureTableData;
		TextureData textureData;
		textureData.width = pTextureTableData->width;
		textureData.height = pTextureTableData->height;
		textureData.size = textureData.width * textureData.height * sizeof(TexelRGBAData);

		void* pTextureBinary = pTextureTableAsset->pTextureTableBinary;
		for (u32 textureIndex = 0; textureIndex < pTextureTableData->count; ++textureIndex)
		{
			TextureResource* pTextureResource = pTextureTableAsset->pTextureResourceArray + textureIndex;
			m_pResourceHandler->AcquireTextureResource(pTextureResource, textureData.width, textureData.height, pTextureBinary);
			pTextureBinary = reinterpret_cast<void*>(reinterpret_cast<u32>(pTextureBinary) + textureData.size);
		}
	}
}

void AssetProcessor::ReleaseAllAssetResources()
{
	for (AssetContainer::TextureIterator textureIterator = m_pAssetContainer->GetTextureIterator(); !textureIterator.IsEnd(); textureIterator.Next())
	{
		TextureAsset* pTextureAsset = textureIterator.GetValue();

		TextureResource* pTextureResource = pTextureAsset->pTextureResource;
		m_pResourceHandler->ReleaseTextureResource(pTextureResource);
	}

	for (AssetContainer::TextureTableIterator textureTableIterator = m_pAssetContainer->GetTextureTableIterator(); !textureTableIterator.IsEnd(); textureTableIterator.Next())
	{
		TextureTableAsset* pTextureTableAsset = textureTableIterator.GetValue();

		TextureTableData* pTextureTableData = pTextureTableAsset->pTextureTableData;

		for (u32 textureIndex = 0; textureIndex < pTextureTableData->count; ++textureIndex)
		{
			TextureResource* pTextureResource = pTextureTableAsset->pTextureResourceArray + textureIndex;
			m_pResourceHandler->ReleaseTextureResource(pTextureResource);
		}
	}
}

//namespace imag
}
//namespace lct
}
