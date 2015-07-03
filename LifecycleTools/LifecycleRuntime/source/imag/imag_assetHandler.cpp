#include <imag/imag_assetHandler.h>
#include <imag/imag_assets.h>
#include <imag/imag_data.h>

#include <grap/grap_device.h>
#include <grap/grap_resources.h>

#include <pack/pack_assetHeader.h>
#include <pack/pack_assetContainer.h>

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

/*
 * Public Instance
 */
AssetHandler::AssetHandler()
: m_pAllocator(NULL)
, m_pGraphicsDevice(NULL)
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

void AssetHandler::SetGraphicsDevice(grap::Device* pGraphicsDevice)
{
	m_pGraphicsDevice = pGraphicsDevice;
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
		if (assetHeader.typeCode.numeric == TEXTURE_TYPE_CODE.numeric)
		{
			TextureAsset* pTextureAsset = LoadTextureAsset(binaryReader);
			m_pAssetContainer->AddAsset(pTextureAsset, pTextureAsset->pTextureData->name);
			assetLoaded = true;
		}
		else if (assetHeader.typeCode.numeric == TEXTURE_TABLE_TYPE_CODE.numeric)
		{
			TextureTableAsset* pTextureTableAsset = LoadTextureTableAsset(binaryReader);
			m_pAssetContainer->AddAsset(pTextureTableAsset, pTextureTableAsset->pTextureTableData->name);
			assetLoaded = true;
		}
	}
	return assetLoaded;
}

TextureAsset* AssetHandler::LoadTextureAsset(util::BinaryReader& binaryReader)
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

TextureTableAsset* AssetHandler::LoadTextureTableAsset(util::BinaryReader& binaryReader)
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

void AssetHandler::AcquireAllAssetResources()
{
	for (pack::AssetIterator<TextureAsset> textureIterator = m_pAssetContainer->GetIterator<TextureAsset>(); !textureIterator.IsEnd(); textureIterator.Next())
	{
		TextureAsset* pTextureAsset = textureIterator.GetAsset();

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

	for (pack::AssetIterator<TextureTableAsset> textureTableIterator = m_pAssetContainer->GetIterator<TextureTableAsset>(); !textureTableIterator.IsEnd(); textureTableIterator.Next())
	{
		TextureTableAsset* pTextureTableAsset = textureTableIterator.GetAsset();

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

void AssetHandler::ReleaseAllAssetResources()
{
	for (pack::AssetIterator<TextureAsset> textureIterator = m_pAssetContainer->GetIterator<TextureAsset>(); !textureIterator.IsEnd(); textureIterator.Next())
	{
		TextureAsset* pTextureAsset = textureIterator.GetAsset();

		grap::TextureResource* pTextureResource = pTextureAsset->pTextureResource;

		grap::TextureSetupParameters textureSetupParameters;
		textureSetupParameters.pTextureResource = pTextureResource;
		m_pGraphicsDevice->ReleaseTextureResource(textureSetupParameters);
	}

	for (pack::AssetIterator<TextureTableAsset> textureTableIterator = m_pAssetContainer->GetIterator<TextureTableAsset>(); !textureTableIterator.IsEnd(); textureTableIterator.Next())
	{
		TextureTableAsset* pTextureTableAsset = textureTableIterator.GetAsset();

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
