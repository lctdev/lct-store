#include <font/font_assetContainer.h>
#include <font/font_assets.h>
#include <font/font_data.h>

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
AssetContainer::AssetContainer()
: m_pAllocator(NULL)
, m_sheetTree()
{
}

void AssetContainer::SetAllocator(lct::foun::Allocator* pAllocator)
{
	m_pAllocator = pAllocator;
}

void AssetContainer::AddSheetAsset(SheetAsset* pSheetAsset)
{
	SheetNode* pNode = m_pAllocator->AllocType<SheetNode>();
	pNode->GetKey().SetString(pSheetAsset->pSheetData->name);
	pNode->SetValue(pSheetAsset);
	m_sheetTree.AddNode(pNode);
}

SheetAsset* AssetContainer::FindSheetAsset(const char* pName)
{
	foun::StringKey key;
	key.SetString(pName);
	SheetNode* pNode = m_sheetTree.FindNode(key);
	if (pNode != NULL)
	{
		return pNode->GetValue();
	}
	else
	{
		return NULL;
	}
}

AssetContainer::SheetIterator AssetContainer::GetSheetIterator()
{
	SheetIterator iterator = m_sheetTree.Begin();
	return iterator;
}

//namespace font
}
//namespace lct
}
