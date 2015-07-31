#ifndef FRAM_MODE_FACTORY_H
#define FRAM_MODE_FACTORY_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>
#include <foun/foun_tree.h>
#include <foun/foun_keys.h>

namespace lct
{
namespace fram
{

class Mode;

class ModeFactoryItem
{
	friend class ModeFactory;

public:
	typedef Mode* (*CreateModeFunc)(lct::foun::Allocator* pAllocator);

	template <class M>
	static Mode* CreateMode(lct::foun::Allocator* pAllocator) { return pAllocator->AllocType<M>(); }

public:
	ModeFactoryItem();

	void SetName(const char* pName);
	void SetCreateModeFunc(CreateModeFunc createModeFunc);
	Mode* CreateMode(lct::foun::Allocator* pAllocator);

private:
	lct::foun::TreeNode<lct::foun::StringKey, ModeFactoryItem*> m_treeNode;
	CreateModeFunc m_createModeFunc;
};

class ModeFactory
{
public:
	ModeFactory();

	void AddItem(ModeFactoryItem* pItem);
	Mode* CreateMode(const char* pName, lct::foun::Allocator* pAllocator);
	u32 GetModeNameCount() { return m_itemTree.GetNodeCount(); }
	void GetAllModeNames(const char** ppModeNameArray, u32 maxCount);

private:
	ModeFactoryItem* FindItem(const lct::foun::StringKey& name);

	lct::foun::Tree<lct::foun::StringKey, ModeFactoryItem*> m_itemTree;
};

//namespace lct
}
//namespace fram
}

#endif//FRAM_MODE_FACTORY_H
