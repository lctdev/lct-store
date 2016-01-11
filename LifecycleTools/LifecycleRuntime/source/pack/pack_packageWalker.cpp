#include <pack/pack_packageWalker.h>

#include <pack/pack_assetHeader.h>
#include <pack/pack_assetHandler.h>

#include <util/util_binaryReader.h>

#include <foun/foun_debug.h>

namespace lct
{
namespace pack
{

/*
 * Internal Constants
 */

/*
 * Public Instance
 */
PackageWalker::PackageWalker()
: m_assetHandlerArray()
, m_assetHandlerCount(0)
{
}

void PackageWalker::AddAssetHandler(IAssetHandler* pAssetHandler)
{
	if (m_assetHandlerCount < MAX_ASSET_HANDLER_COUNT)
	{
		m_assetHandlerArray[m_assetHandlerCount++] = pAssetHandler;
	}
}

void PackageWalker::LoadAllAssets(util::BinaryReader& binaryReader)
{
	pack::AssetHeader* pAssetHeader;
	while (binaryReader.CanReadType<pack::AssetHeader>())
	{
		pAssetHeader = binaryReader.ReadType<pack::AssetHeader>();
		bool assetLoaded = false;
#if defined(LCT_DEBUG)
		ssiz oldPosition = binaryReader.GetPosition();
#endif
		for (u32 handlerIndex = 0; handlerIndex < m_assetHandlerCount; ++handlerIndex)
		{
			pack::IAssetHandler* pAssetHandler = m_assetHandlerArray[handlerIndex];
			assetLoaded = pAssetHandler->LoadAsset(*pAssetHeader, binaryReader);
			if (assetLoaded)
			{
#if defined(LCT_DEBUG)
				ssiz newPosition = binaryReader.GetPosition();
				ssiz offset = newPosition - oldPosition;
				LCT_ASSERT(offset == (s32)pAssetHeader->dataSize);
#endif
				break;
			}
		}
		if (!assetLoaded)
		{
			binaryReader.Read(pAssetHeader->dataSize);
		}
	}
}

//namespace pack
}
//namespace lct
}
