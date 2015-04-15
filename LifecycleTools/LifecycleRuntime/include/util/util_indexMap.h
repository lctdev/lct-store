#ifndef UTIL_INDEX_MAP_H
#define UTIL_INDEX_MAP_H

#include <foun/foun_primitives.h>
#include <foun/foun_tree.h>
#include <foun/foun_keys.h>
#include <foun/foun_allocator.h>

namespace lct
{
namespace util
{

class StringIndexMap
{
public:
	StringIndexMap();

	void AddIndex(const char* pString, u32 index, foun::Allocator* pAllocator);
	s32 FindIndex(const char* pString);

private:
	typedef foun::TreeNode<foun::StringKey, u32> Node;

	foun::Tree<foun::StringKey, u32> m_tree;
};

class IntegerIndexMap
{
public:
	IntegerIndexMap();

	void AddIndex(s32 integer, u32 index, foun::Allocator* pAllocator);
	s32 FindIndex(s32 integer);

private:
	typedef foun::TreeNode<foun::IntegerKey, u32> Node;

	foun::Tree<foun::IntegerKey, u32> m_tree;
};

//namespace util
}
//namespace lct
}

#endif//UTIL_INDEX_MAP_H
