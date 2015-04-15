#ifndef IMAG_ASSET_CONTAINER_H
#define IMAG_ASSET_CONTAINER_H

#include <foun/foun_primitives.h>
#include <foun/foun_tree.h>
#include <foun/foun_keys.h>
#include <foun/foun_allocator.h>

namespace lct
{
namespace imag
{

struct TextureAsset;
struct TextureTableAsset;

class AssetContainer
{
public:
	typedef foun::TreeIterator<lct::foun::StringKey, TextureTableAsset*> TextureTableIterator;
	typedef foun::TreeIterator<lct::foun::StringKey, TextureAsset*> TextureIterator;

	AssetContainer();

	void SetAllocator(foun::Allocator* m_pAllocator);

	void AddTextureAsset(TextureAsset* pTextureAsset);
	TextureAsset* FindTextureAsset(const char* pName);
	TextureIterator GetTextureIterator();

	void AddTextureTableAsset(TextureTableAsset* pTextureTableAsset);
	TextureTableAsset* FindTextureTableAsset(const char* pName);
	TextureTableIterator GetTextureTableIterator();

private:
	typedef foun::TreeNode<lct::foun::StringKey, TextureAsset*> TextureNode;
	typedef foun::TreeNode<lct::foun::StringKey, TextureTableAsset*> TextureTableNode;

	foun::Allocator* m_pAllocator;
	foun::Tree<lct::foun::StringKey, TextureAsset*> m_textureTree;
	foun::Tree<lct::foun::StringKey, TextureTableAsset*> m_textureTableTree;
};

//namespace imag
}
//namespace lct
}

#endif//IMAG_ASSET_CONTAINER_H
