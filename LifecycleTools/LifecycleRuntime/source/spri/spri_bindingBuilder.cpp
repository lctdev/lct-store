#include <spri/spri_bindingBuilder.h>
#include <spri/spri_bindings.h>
#include <spri/spri_data.h>
#include <spri/spri_assets.h>

#include <util/util_indexMap.h>

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
BindingBuilder::BindingBuilder()
: m_pAllocator(NULL)
{
}

void BindingBuilder::SetAllocator(lct::foun::Allocator* pAllocator)
{
	m_pAllocator = pAllocator;
}

FigureSkinBinding* BindingBuilder::CreateFigureSkinBinding(FigureAsset& figureAsset, SkinAsset& skinAsset)
{
	const SkinData* pSkinData = skinAsset.pSkinData;

	const SkinAsset::Strip* pStripArray = skinAsset.pStripArray;
	s32* pPatternIndexArray = m_pAllocator->AllocTypeArray<s32>(pSkinData->stripCount);
	for (u32 stripIndex = 0; stripIndex < pSkinData->stripCount; ++stripIndex)
	{
		const SkinAsset::Strip* pStrip = pStripArray + stripIndex;
		const StripData* pStripData = pStrip->pStripData;
		pPatternIndexArray[stripIndex] = figureAsset.pPatternIndexMap->FindIndex(pStripData->patternName);
	}

	FigureSkinBinding* pFigureSkinBinding = m_pAllocator->AllocType<FigureSkinBinding>();
	pFigureSkinBinding->pFigureAsset = &figureAsset;
	pFigureSkinBinding->pSkinAsset = &skinAsset;
	pFigureSkinBinding->pPatternIndexArray = pPatternIndexArray;

	return pFigureSkinBinding;
}

AnimationFigureBinding* BindingBuilder::CreateAnimationFigureBinding(AnimationAsset& animationAsset, FigureAsset& figureAsset)
{
	const AnimationData* pAnimationData = animationAsset.pAnimationData;

	const AnimationAsset::Track* pTrackArray = animationAsset.pTrackArray;
	s32* pTargetIndexArray = m_pAllocator->AllocTypeArray<s32>(pAnimationData->trackCount);
	for (u32 trackIndex = 0; trackIndex < pAnimationData->trackCount; ++trackIndex)
	{
		const AnimationAsset::Track* pTrack = pTrackArray + trackIndex;
		const TrackData* pTrackData = pTrack->pTrackData;
		switch (pTrackData->targetType)
		{
		case TARGET_TYPE_PART:
			pTargetIndexArray[trackIndex] = figureAsset.pPartIndexMap->FindIndex(pTrackData->targetName);
			break;
		case TARGET_TYPE_JOINT:
			pTargetIndexArray[trackIndex] = figureAsset.pJointIndexMap->FindIndex(pTrackData->targetName);
			break;
		case TARGET_TYPE_PATTERN:
			pTargetIndexArray[trackIndex] = figureAsset.pPatternIndexMap->FindIndex(pTrackData->targetName);
			break;
		default:
			pTargetIndexArray[trackIndex] = -1;
			break;
		}
	}

	AnimationFigureBinding* pAnimationFigureBinding = m_pAllocator->AllocType<AnimationFigureBinding>();
	pAnimationFigureBinding->pAnimationAsset = &animationAsset;
	pAnimationFigureBinding->pFigureAsset = &figureAsset;
	pAnimationFigureBinding->pTargetIndexArray = pTargetIndexArray;

	return pAnimationFigureBinding;
}

//namespace spri
}
//namespace lct
}
