#include <fram/fram_modeFactory.h>

namespace lct
{
namespace fram
{

/*
 * Public Instance
 */
ModeFactoryItem::ModeFactoryItem()
: m_treeNode()
, m_createModeFunc(NULL)
{
	m_treeNode.SetValue(this);
}

void ModeFactoryItem::SetName(const char* pName)
{
	m_treeNode.GetKey().SetString(pName);
}

void ModeFactoryItem::SetCreateModeFunc(CreateModeFunc createModeFunc)
{
	m_createModeFunc = createModeFunc;
}

Mode* ModeFactoryItem::CreateMode(lct::foun::Allocator* pAllocator)
{
	return m_createModeFunc(pAllocator);
}

/*
 * Public Instance
 */
ModeFactory::ModeFactory()
: m_itemTree()
{
}

void ModeFactory::AddItem(ModeFactoryItem* pItem)
{
	m_itemTree.AddNode(&pItem->m_treeNode);
}

Mode* ModeFactory::CreateMode(const char* pName, lct::foun::Allocator* pAllocator)
{
	lct::foun::StringKey nameKey;
	nameKey.SetString(pName);
	ModeFactoryItem* pItem = m_itemTree.FindNode(nameKey)->GetValue();
	return pItem->CreateMode(pAllocator);
}

void ModeFactory::GetAllModeNames(const char** ppModeNameArray, u32 maxCount)
{
	u32 modeNameIndex = 0;
	for (foun::TreeIterator<lct::foun::StringKey, ModeFactoryItem*> iter = m_itemTree.Begin(); !iter.IsEnd(); iter.Next())
	{
		if (modeNameIndex >= maxCount)
		{
			return;
		}

		ppModeNameArray[modeNameIndex++] = iter.GetKey().GetString();
	}
}

/*
 * Private Instance
 */

//namespace lct
}
//namespace fram
}
