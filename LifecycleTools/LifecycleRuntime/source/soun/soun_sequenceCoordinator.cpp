#include <soun/soun_sequenceCoordinator.h>
#include <soun/soun_sequenceInstance.h>
#include <soun/soun_assets.h>
#include <soun/soun_data.h>
#include <soun/soun_handles.h>

namespace lct
{
namespace soun
{

/*
* Internal Constants
*/
static const u32 CLIP_HANDLE_CAPACITY = 4;

/*
* Public Instance
*/
SequenceCoordinator::SequenceCoordinator()
	: m_pClipCoordinator(NULL)
	, m_slotCapacity(0)
	, m_pSlotArray(NULL)
	, m_dormantSlotList()
	, m_activeSlotList()
{
}

void SequenceCoordinator::SetClipCoordinator(ClipCoordinator* pClipCoordinator)
{
	m_pClipCoordinator = pClipCoordinator;
}

void SequenceCoordinator::CreateStructure(u32 slotCapacity, foun::Allocator* pAllocator)
{
	m_slotCapacity = slotCapacity;

	m_pSlotArray = pAllocator->AllocTypeArray<Slot>(m_slotCapacity);
	for (u32 slotIndex = 0; slotIndex < m_slotCapacity; ++slotIndex)
	{
		Slot* pSlot = m_pSlotArray + slotIndex;
		pSlot->listNode.SetValue(pSlot);

		pSlot->pSequenceInstance = pAllocator->AllocType<SequenceInstance>();
		pSlot->pSequenceInstance->SetClipCoordinator(m_pClipCoordinator);
		pSlot->pSequenceInstance->CreateStructure(CLIP_HANDLE_CAPACITY, pAllocator);

		pSlot->pSequenceHandle = NULL;

		m_dormantSlotList.AddNode(&pSlot->listNode);
	}
}

void SequenceCoordinator::BeginSequence(SequenceHandle* pSequenceHandle, const SequenceAsset* pSequenceAsset)
{
	if (pSequenceHandle != NULL)
	{
		Slot* pSlot = GetSlot(pSequenceHandle);
		if (pSlot != NULL)
		{
			return;
		}
	}

	Slot* pSlot = ActivateSlot(pSequenceHandle);
	if (pSlot == NULL)
	{
		// TODO: kill oldest by priority?
		return;
	}

	SequenceInstance* pSequenceInstance = pSlot->pSequenceInstance;

	pSequenceInstance->BindSequenceAsset(pSequenceAsset);
	pSequenceInstance->ResetTimeline();

	pSlot->flags.Set(Slot::FLAG_RUNNING);
}

void SequenceCoordinator::EndSequence(SequenceHandle* pSequenceHandle)
{
	Slot* pSlot = GetSlot(pSequenceHandle);
	if (pSlot == NULL)
	{
		return;
	}

	pSlot->flags.Set(Slot::FLAG_BREAK_PENDING);
}

void SequenceCoordinator::DetachHandle(SequenceHandle* pSequenceHandle)
{
	Slot* pSlot = GetSlot(pSequenceHandle);
	if (pSlot != NULL)
	{
		pSequenceHandle->m_slotIndex = -1;

		pSlot->pSequenceHandle = NULL;
	}
}

void SequenceCoordinator::Update(f32 secondStep)
{
	UpdateSequences(secondStep);
	RecycleSlots();
}

/*
* Protected Instance
*/
void SequenceCoordinator::UpdateSequences(f32 secondStep)
{
	for (foun::ListIterator<Slot*> slotIterator = m_activeSlotList.Head(); !slotIterator.IsEnd(); slotIterator.Next())
	{
		Slot* pSlot = slotIterator.GetValue();
		SequenceInstance* pSequenceInstance = pSlot->pSequenceInstance;
		const SequenceAsset* pSequenceAsset = pSequenceInstance->m_pSequenceAsset;
		const SequenceData* pSequenceData = pSequenceAsset->pSequenceData;

		if (pSlot->flags.Test(Slot::FLAG_BREAK_PENDING))
		{
			// break the sequence; move to the next timeline if we have one, or stop running
			if (pSequenceInstance->m_timelineIndex < (pSequenceData->timelineCount - 1))
			{
				pSequenceInstance->SetTimelineIndex(pSequenceInstance->m_timelineIndex + 1);
			}
			else
			{
				pSlot->flags.Clear(Slot::FLAG_RUNNING);
			}

			pSlot->flags.Clear(Slot::FLAG_BREAK_PENDING);
		}

		if (pSlot->flags.Test(Slot::FLAG_RUNNING))
		{
			pSequenceInstance->UpdateTimeline(secondStep);

			if (pSequenceInstance->IsFinished() && (pSequenceInstance->m_timelineIndex >= (pSequenceData->timelineCount - 1)))
			{
				// finished the last timeline, stop running
				pSlot->flags.Clear(Slot::FLAG_RUNNING);
			}
		}

		if (!pSlot->flags.Test(Slot::FLAG_RUNNING))
		{
			// sequence is no longer active
			pSlot->flags.Clear(Slot::FLAG_ACTIVE);
		}
	}
}

void SequenceCoordinator::RecycleSlots()
{
	foun::ListIterator<Slot*> slotIterator = m_activeSlotList.Head();
	while (!slotIterator.IsEnd())
	{
		Slot* pSlot = slotIterator.GetValue();
		SequenceInstance* pSequenceInstance = pSlot->pSequenceInstance;
		bool deactivate = false;

		if (!pSlot->flags.Test(Slot::FLAG_ACTIVE))
		{
			deactivate = true;
		}

		slotIterator.Next();

		if (deactivate)
		{
			pSlot->flags.ClearAll();

			DeactivateSlot(pSlot);
		}
	}
}

SequenceCoordinator::Slot* SequenceCoordinator::ActivateSlot(SequenceHandle* pSequenceHandle)
{
	Slot* pSlot = NULL;
	if (m_dormantSlotList.GetNodeCount() > 0)
	{
		pSlot = m_dormantSlotList.Tail().GetValue();
		m_dormantSlotList.RemoveNode(&pSlot->listNode);
		m_activeSlotList.AddNode(&pSlot->listNode);
	}

	if (pSequenceHandle != NULL)
	{
		pSequenceHandle->m_slotIndex = pSlot - m_pSlotArray;

		pSlot->pSequenceHandle = pSequenceHandle;
	}

	pSlot->flags.Set(Slot::FLAG_ACTIVE);

	return pSlot;
}

void SequenceCoordinator::DeactivateSlot(Slot* pSlot)
{
	SequenceHandle* pSequenceHandle = pSlot->pSequenceHandle;

	if (pSequenceHandle != NULL)
	{
		pSequenceHandle->m_slotIndex = -1;

		pSlot->pSequenceHandle = NULL;
	}

	pSlot->flags.Clear(Slot::FLAG_ACTIVE);

	m_activeSlotList.RemoveNode(&pSlot->listNode);
	m_dormantSlotList.AddNode(&pSlot->listNode);
}

SequenceCoordinator::Slot* SequenceCoordinator::GetSlot(SequenceHandle* pSequenceHandle)
{
	if (pSequenceHandle->m_slotIndex == -1)
	{
		return NULL;
	}
	else
	{
		Slot* pSlot = m_pSlotArray + pSequenceHandle->m_slotIndex;
		if (pSlot->pSequenceHandle == pSequenceHandle)
		{
			return pSlot;
		}
		else
		{
			return NULL;
		}
	}
}

//namespace soun
}
//namespace lct
}
