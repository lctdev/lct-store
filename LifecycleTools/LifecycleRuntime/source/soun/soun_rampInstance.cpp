#include <soun/soun_rampInstance.h>
#include <soun/soun_clipInstance.h>
#include <soun/soun_data.h>
#include <soun/soun_assets.h>

#include <foun/foun_debug.h>

#include <string.h>

namespace lct
{
namespace soun
{

/*
* Internal Constants
*/

/*
* Public Instance
*/
RampInstance::RampInstance()
: m_pClipInstance(NULL)
, m_pRampAsset(NULL)
, m_trackCapacity(0)
, m_trackCount(0)
, m_pTrackArray(NULL)
{
}

void RampInstance::CreateStructure(u32 trackCapacity, foun::Allocator* pAllocator)
{
	m_trackCapacity = trackCapacity;
	m_trackCount = 0;

	m_pTrackArray = pAllocator->AllocTypeArray<Track>(m_trackCapacity);
}

void RampInstance::BindClipInstance(ClipInstance* pClipInstance)
{
	m_pClipInstance = pClipInstance;

	ClearRampAsset();
}

void RampInstance::ClearRampAsset()
{
	m_pRampAsset = NULL;
	m_trackCount = 0;
	memset(m_pTrackArray, 0, (sizeof(Track) * m_trackCount));
}

void RampInstance::BindRampAsset(const RampAsset* pRampAsset)
{
	RampData* pRampData = pRampAsset->pRampData;

	if (pRampData->trackCount > m_trackCapacity)
	{
		return;
	}

	m_pRampAsset = pRampAsset;

	m_trackCount = 0;
	for (u32 assetTrackIndex = 0; assetTrackIndex < pRampData->trackCount; ++assetTrackIndex)
	{
		const RampAsset::Track& assetTrack = pRampAsset->pTrackArray[assetTrackIndex];
		Track& track = m_pTrackArray[m_trackCount++];
		const TrackData* pTrackData = assetTrack.pTrackData;
		const KeyData* pKeyDataArray = assetTrack.pKeyDataArray;
		LCT_ASSERT(pTrackData->keyCount > 1);
 
		f32* pTargetProperty;
		pTargetProperty = m_pClipInstance->m_propertyArray + pTrackData->targetPropertyType;
		track.pTargetProperty = pTargetProperty;

		const KeyData* pLastKeyData = pKeyDataArray + (pTrackData->keyCount - 1);
		track.secondDuration = pLastKeyData->second;
	}

	ResetTracks();
}

f32 RampInstance::FindLongestTrackSecondDuration()
{
	f32 longestTrackSecondDuration = 0.0f;
	for (u32 trackIndex = 0; trackIndex < m_trackCount; ++trackIndex)
	{
		Track& track = m_pTrackArray[trackIndex];
		if (track.secondDuration > longestTrackSecondDuration)
		{
			longestTrackSecondDuration = track.secondDuration;
		}
	}
	return longestTrackSecondDuration;
}

bool RampInstance::IsFinished()
{
	for (u32 trackIndex = 0; trackIndex < m_trackCount; ++trackIndex)
	{
		Track& track = m_pTrackArray[trackIndex];
		if (track.second < track.secondDuration)
		{
			return false;
		}
	}
	return true;
}

void RampInstance::ResetTracks()
{
	for (u32 trackIndex = 0; trackIndex < m_trackCount; ++trackIndex)
	{
		Track& track = m_pTrackArray[trackIndex];
		const KeyData* pKeyData = m_pRampAsset->pTrackArray[trackIndex].pKeyDataArray;

		track.keyIndex = 0;
		track.second = 0.0f;
		*track.pTargetProperty = pKeyData->value;
	}
}

void RampInstance::UpdateTracks(f32 secondStep)
{	
	for (u32 trackIndex = 0; trackIndex < m_trackCount; ++trackIndex)
	{
		Track& track = m_pTrackArray[trackIndex];
		const TrackData* pTrackData = m_pRampAsset->pTrackArray[trackIndex].pTrackData;

		track.second += secondStep;

		KeyData* pKeyDataArray = m_pRampAsset->pTrackArray[trackIndex].pKeyDataArray;
		const KeyData* pNextKeyData = pKeyDataArray + track.keyIndex + 1;
		u32 endKeyIndex = pTrackData->keyCount - 1;
		while ((pNextKeyData != NULL) && (track.second >= pNextKeyData->second))
		{
			++track.keyIndex;
			if (track.keyIndex < endKeyIndex)
			{
				pNextKeyData = pKeyDataArray + track.keyIndex + 1;
			}
			else
			{
				switch (pTrackData->finishType)
				{
				case FINISH_TYPE_HOLD:
				{
					track.keyIndex = endKeyIndex;
					track.second = track.secondDuration;
					pNextKeyData = NULL;
				}
				break;
				case FINISH_TYPE_LOOP:
				{
					track.keyIndex = 0;
					track.second += pTrackData->loopSecond;
					track.second -= track.secondDuration;
					pNextKeyData = pKeyDataArray + track.keyIndex + 1;
				}
				break;
				}
			}
		}
		const KeyData* pCurrKeyData = pKeyDataArray + track.keyIndex;

		if (pNextKeyData != NULL)
		{
			switch (pCurrKeyData->curveType)
			{
			case CURVE_TYPE_FLAT:
			{
				*track.pTargetProperty = pCurrKeyData->value;
			}
			break;
			case CURVE_TYPE_LINEAR:
			{
				f32 secondDelta = track.second - pCurrKeyData->second;
				f32 valueDelta = secondDelta * pCurrKeyData->slope;
				*track.pTargetProperty = pCurrKeyData->value + valueDelta;
			}
			break;
			}
		}
		else
		{
			*track.pTargetProperty = pCurrKeyData->value;
		}
	}
}

void RampInstance::ForceSecondOnTracks(f32 second)
{
	ResetTracks();
	UpdateTracks(second);
}

//namespace soun
}
//namespace lct
}
