#ifndef PACK_ASSET_CONTAINER_H
#define PACK_ASSET_CONTAINER_H

#include <foun/foun_primitives.h>
#include <foun/foun_tree.h>
#include <foun/foun_keys.h>
#include <foun/foun_allocator.h>

namespace lct
{
namespace pack
{

struct Asset;

template <class T>
class AssetIterator
{
	friend class AssetContainer;

public:
	T* GetAsset() { return static_cast<T*>(m_treeIterator.GetValue()); }
	void Next() { m_treeIterator.Next(); }
	bool IsEnd() { return m_treeIterator.IsEnd(); }

private:
	typedef foun::TreeIterator<foun::StringKey, Asset*> TreeIterator;

	AssetIterator(TreeIterator treeIterator) : m_treeIterator(treeIterator) {}

	TreeIterator m_treeIterator;
};

class AssetContainer
{
public:
	AssetContainer();

	void SetAllocator(foun::Allocator* m_pAllocator);

	template <class T>
	void AddAsset(T* pAsset, const char* pName);
	template <class T>
	T* FindAsset(const char* pName);
	template <class T>
	u32 GetAssetCount();
	template <class T>
	AssetIterator<T> GetIterator();

private:
	foun::Allocator* m_pAllocator;

	typedef foun::TreeNode<foun::StringKey, Asset*> AssetNode;
	typedef foun::Tree<foun::StringKey, Asset*> AssetTree;
	typedef foun::TreeNode<foun::IntegerKey, AssetTree> TypeNode;
	typedef foun::Tree<foun::IntegerKey, AssetTree> TypeTree;
	typedef foun::TreeNode<foun::IntegerKey, TypeTree> GroupNode;
	typedef foun::Tree<foun::IntegerKey, TypeTree> GroupTree;

	GroupNode* AddGroupNode(u32 groupCode);
	TypeNode* AddTypeNode(u32 typeCode, GroupNode* pGroupNode);
	AssetNode* AddAssetNode(const char* pAssetName, Asset* pAsset, TypeNode* pTypeNode);

	GroupNode* FindGroupNode(u32 groupCode);
	TypeNode* FindTypeNode(u32 typeCode, GroupNode* pGroupNode);
	TypeNode* FindTypeNode(u32 groupCode, u32 typeCode);
	AssetNode* FindAssetNode(const char* pAssetName, TypeNode* pTypeNode);

	GroupTree m_groupTree;
};

template <class T>
void AssetContainer::AddAsset(T* pAsset, const char* pAssetName)
{
	u32 groupCode = T::GROUP_CODE.numeric;
	u32 typeCode = T::TYPE_CODE.numeric;

	GroupNode* pGroupNode = FindGroupNode(groupCode);
	if (pGroupNode == NULL)
	{
		pGroupNode = AddGroupNode(groupCode);
	}

	TypeNode* pTypeNode = FindTypeNode(typeCode, pGroupNode);
	if (pTypeNode == NULL)
	{
		pTypeNode = AddTypeNode(typeCode, pGroupNode);
	}

	AddAssetNode(pAssetName, pAsset, pTypeNode);
}

template <class T>
T* AssetContainer::FindAsset(const char* pName)
{
	u32 groupCode = T::GROUP_CODE.numeric;
	u32 typeCode = T::TYPE_CODE.numeric;

	GroupNode* pGroupNode = FindGroupNode(groupCode);
	if (pGroupNode == NULL)
	{
		return NULL;
	}

	TypeNode* pTypeNode = FindTypeNode(typeCode, pGroupNode);
	if (pTypeNode == NULL)
	{
		return NULL;
	}

	AssetNode* pAssetNode = FindAssetNode(pName, pTypeNode);
	if (pAssetNode == NULL)
	{
		return NULL;
	}

	return static_cast<T*>(pAssetNode->GetValue());
}

template <class T>
u32 AssetContainer::GetAssetCount()
{
	u32 groupCode = T::GROUP_CODE.numeric;
	u32 typeCode = T::TYPE_CODE.numeric;

	TypeNode* pTypeNode = FindTypeNode(groupCode, typeCode);
	if (pTypeNode == NULL)
	{
		return 0;
	}
	else
	{
		return pTypeNode->GetValue().GetNodeCount();
	}
}

template <class T>
AssetIterator<T> AssetContainer::GetIterator()
{
	u32 groupCode = T::GROUP_CODE.numeric;
	u32 typeCode = T::TYPE_CODE.numeric;

	TypeNode* pTypeNode = FindTypeNode(groupCode, typeCode);
	if (pTypeNode == NULL)
	{
		return AssetIterator<T>(AssetIterator<T>::TreeIterator::EMPTY);
	}

	return AssetIterator<T>(pTypeNode->GetValue().Begin());
}

//namespace pack
}
//namespace lct
}

#endif//PACK_ASSET_CONTAINER_H
