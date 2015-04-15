#include <spri/spri_instanceBuilder.h>
#include <spri/spri_data.h>
#include <spri/spri_assets.h>
#include <spri/spri_figureInstance.h>
#include <spri/spri_animationInstance.h>

#include <imag/imag_assets.h>

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
InstanceBuilder::InstanceBuilder()
: m_pAllocator(NULL)
{
}

void InstanceBuilder::SetAllocator(lct::foun::Allocator* pAllocator)
{
	m_pAllocator = pAllocator;
}

FigureInstance* InstanceBuilder::CreateFigureInstance(FigureAsset& figureAsset)
{
	const FigureData* pFigureData = figureAsset.pFigureData;

	const FigureAsset::Part* pAssetPartArray = figureAsset.pPartArray;
	FigureInstance::Part* pPartArray = m_pAllocator->AllocTypeArray<FigureInstance::Part>(pFigureData->partCount);
	for (u32 partIndex = 0; partIndex < pFigureData->partCount; ++partIndex)
	{
		const FigureAsset::Part& assetPart = pAssetPartArray[partIndex];
		FigureInstance::Part& part = pPartArray[partIndex];
		part.pPartData = assetPart.pPartData;
	}

	const FigureAsset::Joint* pAssetJointArray = figureAsset.pJointArray;
	FigureInstance::Joint* pJointArray = m_pAllocator->AllocTypeArray<FigureInstance::Joint>(pFigureData->jointCount);
	for (u32 jointIndex = 0; jointIndex < pFigureData->jointCount; ++jointIndex)
	{
		const FigureAsset::Joint& assetJoint = pAssetJointArray[jointIndex];
		FigureInstance::Joint& joint = pJointArray[jointIndex];
		joint.pJointData = assetJoint.pJointData;
	}

	const FigureAsset::Pattern* pAssetPatternArray = figureAsset.pPatternArray;
	FigureInstance::Pattern* pPatternArray = m_pAllocator->AllocTypeArray<FigureInstance::Pattern>(pFigureData->patternCount);
	for (u32 patternIndex = 0; patternIndex < pFigureData->patternCount; ++patternIndex)
	{
		const FigureAsset::Pattern& assetPattern = pAssetPatternArray[patternIndex];
		FigureInstance::Pattern& pattern = pPatternArray[patternIndex];
		pattern.pPatternData = assetPattern.pPatternData;
		imag::TextureTableAsset* pTextureTableAsset = assetPattern.pTextureTableAsset;
		if (pTextureTableAsset != NULL)
		{
			pattern.pTextureTableData = pTextureTableAsset->pTextureTableData;
			pattern.pTextureResourceArray = pTextureTableAsset->pTextureResourceArray;
		}
		else
		{
			pattern.pTextureTableData = NULL;
			pattern.pTextureResourceArray = NULL;
		}
	}

	FigureInstance* pFigureInstance = m_pAllocator->AllocType<FigureInstance>();
	pFigureInstance->m_pFigureAsset = &figureAsset;
	pFigureInstance->m_pPartArray = pPartArray;
	pFigureInstance->m_partCount = pFigureData->partCount;
	pFigureInstance->m_pJointArray = pJointArray;
	pFigureInstance->m_jointCount = pFigureData->jointCount;
	pFigureInstance->m_pPatternArray = pPatternArray;
	pFigureInstance->m_patternCount = pFigureData->patternCount;

	pFigureInstance->ResetProperties();
	pFigureInstance->CalculateTransforms();

	return pFigureInstance;
}

AnimationInstance* InstanceBuilder::CreateAnimationInstance(u32 trackCount)
{
	AnimationInstance::Track* pTrackArray = m_pAllocator->AllocTypeArray<AnimationInstance::Track>(trackCount);

	AnimationInstance* pAnimationInstance = m_pAllocator->AllocType<AnimationInstance>();
	pAnimationInstance->m_pTrackArray = pTrackArray;
	pAnimationInstance->m_trackCount = trackCount;

	return pAnimationInstance;
}

//namespace spri
}
//namespace lct
}
