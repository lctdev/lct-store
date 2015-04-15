#include <imag/imag_assetContainer.h>
#include <imag/imag_assets.h>
#include <imag/imag_data.h>

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
AssetContainer::AssetContainer()
: m_pAllocator(NULL)
, m_textureTableTree()
{
}

void AssetContainer::SetAllocator(lct::foun::Allocator* pAllocator)
{
	m_pAllocator = pAllocator;
}

void AssetContainer::AddTextureAsset(TextureAsset* pTextureAsset)
{
	TextureNode* pNode = m_pAllocator->AllocType<TextureNode>();
	pNode->GetKey().SetString(pTextureAsset->pTextureData->name);
	pNode->SetValue(pTextureAsset);
	m_textureTree.AddNode(pNode);
}

TextureAsset* AssetContainer::FindTextureAsset(const char* pName)
{
	foun::StringKey key;
	key.SetString(pName);
	TextureNode* pNode = m_textureTree.FindNode(key);
	if (pNode != NULL)
	{
		return pNode->GetValue();
	}
	else
	{
		return NULL;
	}
}

AssetContainer::TextureIterator AssetContainer::GetTextureIterator()
{
	TextureIterator iterator = m_textureTree.Begin();
	return iterator;
}

void AssetContainer::AddTextureTableAsset(TextureTableAsset* pTextureTableAsset)
{
	TextureTableNode* pNode = m_pAllocator->AllocType<TextureTableNode>();
	pNode->GetKey().SetString(pTextureTableAsset->pTextureTableData->name);
	pNode->SetValue(pTextureTableAsset);
	m_textureTableTree.AddNode(pNode);
}

TextureTableAsset* AssetContainer::FindTextureTableAsset(const char* pName)
{
	foun::StringKey key;
	key.SetString(pName);
	TextureTableNode* pNode = m_textureTableTree.FindNode(key);
	if (pNode != NULL)
	{
		return pNode->GetValue();
	}
	else
	{
		return NULL;
	}
}

AssetContainer::TextureTableIterator AssetContainer::GetTextureTableIterator()
{
	TextureTableIterator iterator = m_textureTableTree.Begin();
	return iterator;
}

//namespace imag
}
//namespace lct
}
