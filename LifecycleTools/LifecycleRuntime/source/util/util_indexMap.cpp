#include <util/util_indexMap.h>

namespace lct
{
namespace util
{

/*
 * Internal Constants
 */

/*
 * Public Instance
 */
StringIndexMap::StringIndexMap()
: m_tree()
{
}

void StringIndexMap::AddIndex(const char* pString, u32 index, foun::Allocator* pAllocator)
{
	Node* pNode = pAllocator->AllocType<Node>();
	pNode->GetKey().SetString(pString);
	pNode->SetValue(index);
	m_tree.AddNode(pNode);
}

s32 StringIndexMap::FindIndex(const char* pString)
{
	foun::StringKey key;
	key.SetString(pString);
	Node* pNode = m_tree.FindNode(key);
	if (pNode != NULL)
	{
		return pNode->GetValue();
	}
	else
	{
		return -1;
	}
}


/*
 * Public Instance
 */
IntegerIndexMap::IntegerIndexMap()
: m_tree()
{
}

void IntegerIndexMap::AddIndex(s32 integer, u32 index, foun::Allocator* pAllocator)
{
	Node* pNode = pAllocator->AllocType<Node>();
	pNode->GetKey().SetInteger(integer);
	pNode->SetValue(index);
	m_tree.AddNode(pNode);
}

s32 IntegerIndexMap::FindIndex(s32 integer)
{
	foun::IntegerKey key;
	key.SetInteger(integer);
	Node* pNode = m_tree.FindNode(key);
	if (pNode != NULL)
	{
		return pNode->GetValue();
	}
	else
	{
		return -1;
	}
}

//namespace util
}
//namespace lct
}
