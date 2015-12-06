#include <soun/soun_sequenceInstance.h>
#include <soun/soun_clipCoordinator.h>
#include <soun/soun_data.h>
#include <soun/soun_assets.h>
#include <soun/soun_handles.h>

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
SequenceInstance::SequenceInstance()
: m_pClipCoordinator(NULL)
, m_clipHandleCapacity(0)
, m_pClipHandleArray(NULL)
, m_pSequenceAsset(NULL)
, m_timeline()
{
}

void SequenceInstance::SetClipCoordinator(ClipCoordinator* pClipCoordinator)
{
	m_pClipCoordinator = pClipCoordinator;
}

void SequenceInstance::CreateStructure(u32 clipHandleCapacity, foun::Allocator* pAllocator)
{
	m_clipHandleCapacity = clipHandleCapacity;

	m_pClipHandleArray = pAllocator->AllocTypeArray<ClipHandle>(m_clipHandleCapacity);
}

void SequenceInstance::ClearSequenceAsset()
{
	m_pSequenceAsset = NULL;
	memset(&m_timeline, 0, sizeof(m_timeline));
}

void SequenceInstance::BindSequenceAsset(const SequenceAsset* pSequenceAsset)
{
	SequenceData* pSequenceData = pSequenceAsset->pSequenceData;

	if (pSequenceData->clipHandleCount > m_clipHandleCapacity)
	{
		return;
	}

	m_pSequenceAsset = pSequenceAsset;

	LCT_ASSERT(pSequenceData->clipHandleCount <= m_clipHandleCapacity);
	
	for (u32 assetTimelineIndex = 0; assetTimelineIndex < pSequenceData->timelineCount; ++assetTimelineIndex)
	{
		const SequenceAsset::Timeline& assetTimeline = pSequenceAsset->pTimelineArray[assetTimelineIndex];
		const TimelineData* pTimelineData = assetTimeline.pTimelineData;
		LCT_ASSERT(pTimelineData->actionCount > 0);
	}	

	LCT_ASSERT(pSequenceData->timelineCount > 0);
	SetTimelineIndex(0);
}

bool SequenceInstance::IsFinished()
{
	return (m_timeline.second >= m_timeline.secondDuration);
}

void SequenceInstance::SetTimelineIndex(u32 timelineIndex)
{
	m_timelineIndex = timelineIndex;

	const SequenceAsset::Timeline& assetTimeline = m_pSequenceAsset->pTimelineArray[m_timelineIndex];
	const TimelineData* pTimelineData = assetTimeline.pTimelineData;
	const SequenceAsset::Action* pAssetActionArray = assetTimeline.pActionArray;
	const SequenceAsset::Action* pLastAssetAction = pAssetActionArray + (pTimelineData->actionCount - 1);
	m_timeline.secondDuration = pLastAssetAction->pActionData->second;

	ResetTimeline();
}

void SequenceInstance::ResetTimeline()
{
	m_timeline.actionIndex = 0;
	m_timeline.second = 0.0f;
}

void SequenceInstance::UpdateTimeline(f32 secondStep)
{	
	m_timeline.second += secondStep;

	const SequenceAsset::Timeline& assetTimeline = m_pSequenceAsset->pTimelineArray[m_timelineIndex];
	const TimelineData* pTimelineData = assetTimeline.pTimelineData;
	const SequenceAsset::Action* pAssetActionArray = assetTimeline.pActionArray;
	const SequenceAsset::Action* pCurrAssetAction = m_timeline.actionIndex < pTimelineData->actionCount ? pAssetActionArray + m_timeline.actionIndex : NULL;

	while ((pCurrAssetAction != NULL) && (m_timeline.second >= pCurrAssetAction->pActionData->second))
	{
		const ActionData* pCurrActionData = pCurrAssetAction->pActionData;
		switch (pCurrActionData->type)
		{
		case ACTION_TYPE_BEGIN_CLIP:
		{
			const BeginClipActionData* pBeginClipActionData = static_cast<const BeginClipActionData*>(pCurrActionData);
			ClipHandle* pClipHandle = m_pClipHandleArray + pBeginClipActionData->handleIndex;
			ClipAsset* pClipAsset = m_pSequenceAsset->pClipReferenceArray[pBeginClipActionData->clipIndex].pClipAsset;

			m_pClipCoordinator->DetachHandle(pClipHandle);
			m_pClipCoordinator->BeginClip(pClipHandle, pClipAsset);
		}
		break;
		case ACTION_TYPE_END_CLIP:
		{
			const EndClipActionData* pEndClipActionData = static_cast<const EndClipActionData*>(pCurrActionData);
			ClipHandle* pClipHandle = m_pClipHandleArray + pEndClipActionData->handleIndex;

			m_pClipCoordinator->EndClip(pClipHandle);
		}
		break;
		case ACTION_TYPE_SET_RAMP:
		{
			const SetRampActionData* pSetRampActionData = static_cast<const SetRampActionData*>(pCurrActionData);
			ClipHandle* pClipHandle = m_pClipHandleArray + pSetRampActionData->handleIndex;
			RampAsset* pRampAsset = m_pSequenceAsset->pRampReferenceArray[pSetRampActionData->rampIndex].pRampAsset;

			m_pClipCoordinator->SetRamp(pClipHandle, pRampAsset);
		}
		break;
		}

		++m_timeline.actionIndex;
		if (m_timeline.actionIndex < pTimelineData->actionCount)
		{
			pCurrAssetAction = pAssetActionArray + m_timeline.actionIndex;
		}
		else
		{
			switch (pTimelineData->finishType)
			{
			case FINISH_TYPE_HOLD:
			{
				m_timeline.second = m_timeline.secondDuration;
				pCurrAssetAction = NULL;
			}
			break;
			case FINISH_TYPE_LOOP:
			{
				m_timeline.actionIndex = 0;
				m_timeline.second = pTimelineData->loopSecond;
				pCurrAssetAction = pAssetActionArray + m_timeline.actionIndex;
				while (m_timeline.second > pCurrAssetAction->pActionData->second)
				{
					++m_timeline.actionIndex;
					pCurrAssetAction = pAssetActionArray + m_timeline.actionIndex;
				}
			}
			break;
			}
		}
	}
}

//namespace soun
}
//namespace lct
}
