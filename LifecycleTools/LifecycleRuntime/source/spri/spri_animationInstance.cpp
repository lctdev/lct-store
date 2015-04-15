#include <spri/spri_animationInstance.h>
#include <spri/spri_figureInstance.h>
#include <spri/spri_data.h>
#include <spri/spri_assets.h>
#include <spri/spri_bindings.h>

#include <foun/foun_debug.h>

#include <string.h>

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
AnimationInstance::AnimationInstance()
: m_pFigureInstance(NULL)
, m_pAnimationAsset(NULL)
, m_pTrackArray(NULL)
, m_trackCount(0)
, m_activeTrackCount(0)
{
}

void AnimationInstance::BindFigureInstance(FigureInstance& figureInstance)
{
	m_pFigureInstance = &figureInstance;

	ClearAnimationAsset();
}

void AnimationInstance::ClearAnimationAsset()
{
	m_pAnimationAsset = NULL;
	m_activeTrackCount = 0;
	memset(m_pTrackArray, 0, (sizeof(Track) * m_trackCount));
}

void AnimationInstance::BindAnimationAsset(AnimationAsset& animationAsset, AnimationFigureBinding& animationFigureBinding)
{
	FigureAsset* pFigureAsset = m_pFigureInstance->m_pFigureAsset;
	AnimationAsset* pAnimationAsset = &animationAsset;

	if ((animationFigureBinding.pFigureAsset != pFigureAsset) || (animationFigureBinding.pAnimationAsset != pAnimationAsset))
	{
		return;
	}

	AnimationData* pAnimationData = animationAsset.pAnimationData;

	if (pAnimationData->trackCount > m_trackCount)
	{
		return;
	}

	m_pAnimationAsset = pAnimationAsset;

	m_activeTrackCount = 0;
	for (u32 assetTrackIndex = 0; assetTrackIndex < pAnimationData->trackCount; ++assetTrackIndex)
	{
		s32 targetIndex = animationFigureBinding.pTargetIndexArray[assetTrackIndex];
		if (targetIndex >= 0)
		{
			const AnimationAsset::Track& assetTrack = animationAsset.pTrackArray[assetTrackIndex];
			Track& track = m_pTrackArray[m_activeTrackCount++];
			const TrackData* pTrackData = assetTrack.pTrackData;
			const KeyData* pKeyDataArray = assetTrack.pKeyDataArray;
			LCT_ASSERT(pTrackData->keyCount > 1);
			track.pTrackData = pTrackData;
			track.pKeyDataArray = pKeyDataArray;

			f32* pTargetProperty;
			switch (pTrackData->targetType)
			{
			case TARGET_TYPE_PART:
			{
				FigureInstance::Part& part = m_pFigureInstance->m_pPartArray[targetIndex];
				pTargetProperty = part.propertyArray + pTrackData->targetPropertyType;
				break;
			}
			case TARGET_TYPE_JOINT:
			{
				FigureInstance::Joint& joint = m_pFigureInstance->m_pJointArray[targetIndex];
				pTargetProperty = joint.propertyArray + pTrackData->targetPropertyType;
				break;
			}
			case TARGET_TYPE_PATTERN:
			{
				FigureInstance::Pattern& pattern = m_pFigureInstance->m_pPatternArray[targetIndex];
				pTargetProperty = pattern.propertyArray + pTrackData->targetPropertyType;
				break;
			}
			default:
				pTargetProperty = NULL;
				break;
			}
			track.pTargetProperty = pTargetProperty;

			const KeyData* pLastKeyData = pKeyDataArray + (pTrackData->keyCount - 1);
			track.frameDuration = pLastKeyData->frame;
		}
	}

	ResetTracks();
}

f32 AnimationInstance::FindLongestTrackFrameDuration()
{
	f32 longestTrackFrameDuration = 0.0f;
	for (u32 trackIndex = 0; trackIndex < m_activeTrackCount; ++trackIndex)
	{
		Track& track = m_pTrackArray[trackIndex];
		if (track.frameDuration > longestTrackFrameDuration)
		{
			longestTrackFrameDuration = track.frameDuration;
		}
	}
	return longestTrackFrameDuration;
}

void AnimationInstance::ResetTracks()
{
	for (u32 trackIndex = 0; trackIndex < m_activeTrackCount; ++trackIndex)
	{
		Track& track = m_pTrackArray[trackIndex];
		const KeyData* pKeyData = track.pKeyDataArray;

		track.keyIndex = 0;
		track.frame = 0.0f;
		*track.pTargetProperty = pKeyData->value;
	}
}

void AnimationInstance::UpdateTracks(f32 frameStep)
{
	for (u32 trackIndex = 0; trackIndex < m_activeTrackCount; ++trackIndex)
	{
		Track& track = m_pTrackArray[trackIndex];
		const TrackData* pTrackData = track.pTrackData;

		track.frame += frameStep;

		const KeyData* pNextKeyData = track.pKeyDataArray + track.keyIndex + 1;
		u32 endKeyIndex = pTrackData->keyCount - 1;
		while ((pNextKeyData != NULL) && (track.frame >= pNextKeyData->frame))
		{
			++track.keyIndex;
			if (track.keyIndex < endKeyIndex)
			{
				pNextKeyData = track.pKeyDataArray + track.keyIndex + 1;
			}
			else
			{
				switch (pTrackData->finishType)
				{
				case FINISH_TYPE_HOLD:
				{
					track.keyIndex = endKeyIndex;
					track.frame = track.frameDuration;
					pNextKeyData = NULL;
				}
				break;
				case FINISH_TYPE_LOOP:
				{
					track.keyIndex = 0;
					track.frame -= track.frameDuration;
					pNextKeyData = track.pKeyDataArray + track.keyIndex + 1;
				}
				break;
				}
			}
		}
		const KeyData* pCurrKeyData = track.pKeyDataArray + track.keyIndex;

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
				f32 frameDelta = track.frame - pCurrKeyData->frame;
				f32 valueDelta = frameDelta * pCurrKeyData->slope;
				*track.pTargetProperty = pCurrKeyData->value + valueDelta;
			}
			break;
			case CURVE_TYPE_HERMITE:
			{
				f32 frameDelta = track.frame - pCurrKeyData->frame;
				f32 frameSpan = pNextKeyData->frame - pCurrKeyData->frame;
				f32 t = frameDelta / frameSpan;
				f32 tSqu = t * t;
				f32 tCub = t * t * t;
				f32 basis1 = (2.0f * tCub) - (3.0f * tSqu) + 1.0f;
				f32 basis2 = -(2.0f * tCub) + (3.0f * tSqu);
				f32 basis3 = tCub - (2.0f * tSqu) + t;
				f32 basis4 = tCub - tSqu;
				f32 value = (basis1 * pCurrKeyData->value) + (basis2 * pNextKeyData->value) + (basis3 * pCurrKeyData->slope) + (basis4 * pNextKeyData->slope);
				*track.pTargetProperty = value;
			}
			}
		}
		else
		{
			*track.pTargetProperty = pCurrKeyData->value;
		}
	}
}

void AnimationInstance::ForceFrameOnTracks(f32 frame)
{
	ResetTracks();
	UpdateTracks(frame);
}

//namespace lct
}
//namespace fram
}
