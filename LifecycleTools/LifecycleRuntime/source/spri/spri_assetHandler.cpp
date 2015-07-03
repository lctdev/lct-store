#include <spri/spri_assetHandler.h>
#include <spri/spri_assets.h>
#include <spri/spri_data.h>
#include <spri/spri_constants.h>

#include <imag/imag_assets.h>
#include <imag/imag_data.h>

#include <pack/pack_assetContainer.h>
#include <pack/pack_assetHeader.h>

#include <util/util_binaryReader.h>
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
AssetHandler::AssetHandler()
: m_pAllocator(NULL)
, m_pAssetContainer(NULL)
{
}

AssetHandler::~AssetHandler()
{
}

void AssetHandler::SetAllocator(lct::foun::Allocator* pAllocator)
{
	m_pAllocator = pAllocator;
}

void AssetHandler::SetAssetContainer(pack::AssetContainer* pAssetContainer)
{
	m_pAssetContainer = pAssetContainer;
}

bool AssetHandler::LoadAsset(pack::AssetHeader& assetHeader, util::BinaryReader& binaryReader)
{
	bool assetLoaded = false;
	if ((assetHeader.groupCode.numeric == GROUP_CODE.numeric) && (assetHeader.version == VERSION))
	{
		if (assetHeader.typeCode.numeric == FIGURE_TYPE_CODE.numeric)
		{
			FigureAsset* pFigureAsset = LoadFigureAsset(binaryReader);
			m_pAssetContainer->AddAsset(pFigureAsset, pFigureAsset->pFigureData->name);
			assetLoaded = true;
		}
		else if (assetHeader.typeCode.numeric == ANIMATION_TYPE_CODE.numeric)
		{
			AnimationAsset* pAnimationAsset = LoadAnimationAsset(binaryReader);
			m_pAssetContainer->AddAsset(pAnimationAsset, pAnimationAsset->pAnimationData->name);
			assetLoaded = true;
		}
	}
	return assetLoaded;
}

FigureAsset* AssetHandler::LoadFigureAsset(util::BinaryReader& binaryReader)
{
	FigureData* pFigureData = binaryReader.ReadType<FigureData>();

	FigureAsset::Part* pPartArray = m_pAllocator->AllocTypeArray<FigureAsset::Part>(pFigureData->partCount);
	util::StringIndexMap* pPartIndexMap = m_pAllocator->AllocType<util::StringIndexMap>();
	for (u32 partIndex = 0; partIndex < pFigureData->partCount; ++partIndex)
	{
		PartData* pPartData = binaryReader.ReadType<PartData>();

		FigureAsset::Part* pPart = pPartArray + partIndex;
		pPart->pPartData = pPartData;

		pPartIndexMap->AddIndex(pPartData->name, partIndex, m_pAllocator);
	}

	FigureAsset::Joint* pJointArray = m_pAllocator->AllocTypeArray<FigureAsset::Joint>(pFigureData->jointCount);
	util::StringIndexMap* pJointIndexMap = m_pAllocator->AllocType<util::StringIndexMap>();
	for (u32 jointIndex = 0; jointIndex < pFigureData->jointCount; ++jointIndex)
	{
		JointData* pJointData = binaryReader.ReadType<JointData>();

		FigureAsset::Joint* pJoint = pJointArray + jointIndex;
		pJoint->pJointData = pJointData;

		pJointIndexMap->AddIndex(pJointData->name, jointIndex, m_pAllocator);
	}

	FigureAsset::Pattern* pPatternArray = m_pAllocator->AllocTypeArray<FigureAsset::Pattern>(pFigureData->patternCount);
	util::StringIndexMap* pPatternIndexMap = m_pAllocator->AllocType<util::StringIndexMap>();
	for (u32 patternIndex = 0; patternIndex < pFigureData->patternCount; ++patternIndex)
	{
		PatternData* pPatternData = binaryReader.ReadType<PatternData>();

		FigureAsset::Pattern* pPattern = pPatternArray + patternIndex;
		pPattern->pPatternData = pPatternData;

		pPatternIndexMap->AddIndex(pPatternData->name, patternIndex, m_pAllocator);
	}

	FigureAsset* pFigureAsset = m_pAllocator->AllocType<FigureAsset>();
	pFigureAsset->pFigureData = pFigureData;
	pFigureAsset->pPartArray = pPartArray;
	pFigureAsset->pJointArray = pJointArray;
	pFigureAsset->pPatternArray = pPatternArray;
	pFigureAsset->pPartIndexMap = pPartIndexMap;
	pFigureAsset->pJointIndexMap = pJointIndexMap;
	pFigureAsset->pPatternIndexMap = pPatternIndexMap;

	return pFigureAsset;
}

AnimationAsset* AssetHandler::LoadAnimationAsset(util::BinaryReader& binaryReader)
{
	AnimationData* pAnimationData = binaryReader.ReadType<AnimationData>();

	AnimationAsset::Track* pTrackArray = m_pAllocator->AllocTypeArray<AnimationAsset::Track>(pAnimationData->trackCount);
	for (u32 trackIndex = 0; trackIndex < pAnimationData->trackCount; ++trackIndex)
	{
		TrackData* pTrackData = binaryReader.ReadType<TrackData>();
		KeyData* pKeyDataArray = binaryReader.ReadTypeArray<KeyData>(pTrackData->keyCount);

		AnimationAsset::Track* pTrack = pTrackArray + trackIndex;
		pTrack->pTrackData = pTrackData;
		pTrack->pKeyDataArray = pKeyDataArray;
	}

	AnimationAsset* pAnimationAsset = m_pAllocator->AllocType<AnimationAsset>();
	pAnimationAsset->pAnimationData = pAnimationData;
	pAnimationAsset->pTrackArray = pTrackArray;

	return pAnimationAsset;
}

void AssetHandler::FixupAllAssets()
{
	for (pack::AssetIterator<FigureAsset> figureIterator = m_pAssetContainer->GetIterator<FigureAsset>(); !figureIterator.IsEnd(); figureIterator.Next())
	{
		FigureAsset* pFigureAsset = figureIterator.GetAsset();
		FigureData* pFigureData = pFigureAsset->pFigureData;
		for (u32 patternIndex = 0; patternIndex < pFigureData->patternCount; ++patternIndex)
		{
			FigureAsset::Pattern* pPattern = pFigureAsset->pPatternArray + patternIndex;
			PatternData* pPatternData = pPattern->pPatternData;
			imag::TextureTableAsset* pTextureTableAsset = m_pAssetContainer->FindAsset<imag::TextureTableAsset>(pPatternData->textureTableName);
			pPattern->pTextureTableAsset = pTextureTableAsset;
		}
	}
}

//namespace spri
}
//namespace lct
}
