#ifndef FONT_ASSET_CONTAINER_H
#define FONT_ASSET_CONTAINER_H

#include <foun/foun_primitives.h>
#include <foun/foun_tree.h>
#include <foun/foun_keys.h>
#include <foun/foun_allocator.h>

namespace lct
{
namespace font
{

struct SheetAsset;

class AssetContainer
{
public:
	typedef foun::TreeIterator<lct::foun::StringKey, SheetAsset*> SheetIterator;

	AssetContainer();

	void SetAllocator(foun::Allocator* m_pAllocator);

	void AddSheetAsset(SheetAsset* pSheetAsset);
	SheetAsset* FindSheetAsset(const char* pName);
	SheetIterator GetSheetIterator();

private:
	typedef foun::TreeNode<lct::foun::StringKey, SheetAsset*> SheetNode;

	foun::Allocator* m_pAllocator;
	foun::Tree<lct::foun::StringKey, SheetAsset*> m_sheetTree;
};

//namespace font
}
//namespace lct
}

#endif//FONT_ASSET_CONTAINER_H
