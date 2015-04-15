#include <spri/spri_assetContainer.h>
#include <spri/spri_assets.h>
#include <spri/spri_data.h>

namespace lct
{
namespace spri
{

/*
 * Internal Constants
 */

/*
 * Public Instance
 */
AssetContainer::AssetContainer()
: m_pAllocator(NULL)
, m_figureTree()
, m_animationTree()
{
}

void AssetContainer::SetAllocator(lct::foun::Allocator* pAllocator)
{
	m_pAllocator = pAllocator;
}

void AssetContainer::AddFigureAsset(FigureAsset* pFigureAsset)
{
	FigureNode* pNode = m_pAllocator->AllocType<FigureNode>();
	pNode->GetKey().SetString(pFigureAsset->pFigureData->name);
	pNode->SetValue(pFigureAsset);
	m_figureTree.AddNode(pNode);
}

FigureAsset* AssetContainer::FindFigureAsset(const char* pName)
{
	foun::StringKey key;
	key.SetString(pName);
	FigureNode* pNode = m_figureTree.FindNode(key);
	if (pNode != NULL)
	{
		return pNode->GetValue();
	}
	else
	{
		return NULL;
	}
}

AssetContainer::FigureIterator AssetContainer::GetFigureIterator()
{
	FigureIterator iterator = m_figureTree.Begin();
	return iterator;
}

void AssetContainer::AddAnimationAsset(AnimationAsset* pAnimationAsset)
{
	AnimationNode* pNode = m_pAllocator->AllocType<AnimationNode>();
	pNode->GetKey().SetString(pAnimationAsset->pAnimationData->name);
	pNode->SetValue(pAnimationAsset);
	m_animationTree.AddNode(pNode);
}

AnimationAsset* AssetContainer::FindAnimationAsset(const char* pName)
{
	foun::StringKey key;
	key.SetString(pName);
	AnimationNode* pNode = m_animationTree.FindNode(key);
	if (pNode != NULL)
	{
		return pNode->GetValue();
	}
	else
	{
		return NULL;
	}
}

AssetContainer::AnimationIterator AssetContainer::GetAnimationIterator()
{
	AnimationIterator iterator = m_animationTree.Begin();
	return iterator;
}

u32 AssetContainer::GetAnimationCount()
{
	u32 animationCount = m_animationTree.GetNodeCount();
	return animationCount;
}

//namespace spri
}
//namespace lct
}
