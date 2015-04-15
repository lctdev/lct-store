#ifndef SPRI_ASSET_CONTAINER_H
#define SPRI_ASSET_CONTAINER_H

#include <foun/foun_primitives.h>
#include <foun/foun_tree.h>
#include <foun/foun_keys.h>
#include <foun/foun_allocator.h>

namespace lct
{
namespace spri
{

struct FigureAsset;
struct AnimationAsset;

class AssetContainer
{
public:
	typedef foun::TreeIterator<lct::foun::StringKey, FigureAsset*> FigureIterator;
	typedef foun::TreeIterator<lct::foun::StringKey, AnimationAsset*> AnimationIterator;

	AssetContainer();

	void SetAllocator(foun::Allocator* m_pAllocator);

	void AddFigureAsset(FigureAsset* pFigureAsset);
	FigureAsset* FindFigureAsset(const char* pName);
	FigureIterator GetFigureIterator();

	void AddAnimationAsset(AnimationAsset* pAnimationAsset);
	AnimationAsset* FindAnimationAsset(const char* pName);
	AnimationIterator GetAnimationIterator();
	u32 GetAnimationCount();

private:
	typedef foun::TreeNode<lct::foun::StringKey, FigureAsset*> FigureNode;
	typedef foun::TreeNode<lct::foun::StringKey, AnimationAsset*> AnimationNode;

	foun::Allocator* m_pAllocator;
	foun::Tree<lct::foun::StringKey, FigureAsset*> m_figureTree;
	foun::Tree<lct::foun::StringKey, AnimationAsset*> m_animationTree;
};

//namespace spri
}
//namespace lct
}

#endif//SPRI_ASSET_CONTAINER_H
