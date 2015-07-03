#include <pass/pass_assetHandler.h>
#include <pass/pass_assets.h>
#include <pass/pass_data.h>

#include <pack/pack_assetHeader.h>
#include <pack/pack_assetContainer.h>

#include <util/util_binaryReader.h>
#include <util/util_indexMap.h>

#include <string.h>

namespace lct
{
namespace pass
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
		if (assetHeader.typeCode.numeric == STRING_TYPE_CODE.numeric)
		{
			StringAsset* pStringAsset = LoadStringAsset(binaryReader);
			m_pAssetContainer->AddAsset(pStringAsset, pStringAsset->pStringData->name);
			assetLoaded = true;
		}
	}
	return assetLoaded;
}

StringAsset* AssetHandler::LoadStringAsset(util::BinaryReader& binaryReader)
{
	StringData* pStringData = binaryReader.ReadType<StringData>();

	char* pString = reinterpret_cast<char*>(binaryReader.Read(pStringData->size));

	StringAsset* pStringAsset = m_pAllocator->AllocType<StringAsset>();
	pStringAsset->pStringData = pStringData;
	pStringAsset->pString = pString;

	return pStringAsset;
}

//namespace pass
}
//namespace lct
}
