#include <pack/pack_assetContainer.h>

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
AssetContainer::AssetContainer()
	: m_pAllocator(NULL)
	, m_groupTree()
{
}

void AssetContainer::SetAllocator(lct::foun::Allocator* pAllocator)
{
	m_pAllocator = pAllocator;
}

/*
* Private Instance
*/
AssetContainer::GroupNode* AssetContainer::AddGroupNode(u32 groupCode)
{
	GroupNode* pGroupNode = m_pAllocator->AllocType<GroupNode>();
	pGroupNode->GetKey().SetInteger(groupCode);
	m_groupTree.AddNode(pGroupNode);
	return pGroupNode;
}

AssetContainer::TypeNode* AssetContainer::AddTypeNode(u32 typeCode, GroupNode* pGroupNode)
{
	TypeNode* pTypeNode = m_pAllocator->AllocType<TypeNode>();
	pTypeNode->GetKey().SetInteger(typeCode);
	pGroupNode->GetValue().AddNode(pTypeNode);
	return pTypeNode;
}

AssetContainer::AssetNode* AssetContainer::AddAssetNode(const char* pAssetName, Asset* pAsset, TypeNode* pTypeNode)
{
	AssetNode* pAssetNode = m_pAllocator->AllocType<AssetNode>();
	pAssetNode->GetKey().SetString(pAssetName);
	pAssetNode->SetValue(pAsset);
	pTypeNode->GetValue().AddNode(pAssetNode);
	return pAssetNode;
}

AssetContainer::GroupNode* AssetContainer::FindGroupNode(u32 groupCode)
{
	GroupNode* pGroupNode = m_groupTree.FindNode(foun::IntegerKey(groupCode));
	return pGroupNode;
}

AssetContainer::TypeNode* AssetContainer::FindTypeNode(u32 typeCode, GroupNode* pGroupNode)
{
	TypeNode* pTypeNode = pGroupNode->GetValue().FindNode(foun::IntegerKey(typeCode));
	return pTypeNode;
}

AssetContainer::TypeNode* AssetContainer::FindTypeNode(u32 groupCode, u32 typeCode)
{
	GroupNode* pGroupNode = m_groupTree.FindNode(foun::IntegerKey(groupCode));
	if (pGroupNode == NULL)
	{
		return NULL;
	}

	TypeNode* pTypeNode = pGroupNode->GetValue().FindNode(foun::IntegerKey(typeCode));
	return pTypeNode;
}

AssetContainer::AssetNode* AssetContainer::FindAssetNode(const char* pAssetName, TypeNode* pTypeNode)
{
	AssetNode* pAssetNode = pTypeNode->GetValue().FindNode(foun::StringKey(pAssetName));
	return pAssetNode;
}

//namespace pack
}
//namespace lct
}
