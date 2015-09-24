#include <soun/soun_clipCoordinator.h>
#include <soun/soun_clipInstance.h>
#include <soun/soun_rampInstance.h>
#include <soun/soun_assets.h>
#include <soun/soun_data.h>

#include <audi/audi_device.h>

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
ClipCoordinator::ClipCoordinator()
: m_slotCapacity(0)
, m_pSlotArray(NULL)
, m_dormantSlotList()
, m_activeSlotList()
{
}

void ClipCoordinator::CreateStructure(u32 slotCapacity, foun::Allocator* pAllocator)
{
	m_slotCapacity = slotCapacity;

	m_pSlotArray = pAllocator->AllocTypeArray<Slot>(m_slotCapacity);
	for (u32 slotIndex = 0; slotIndex < m_slotCapacity; ++slotIndex)
	{
		Slot* pSlot = m_pSlotArray + slotIndex;
		pSlot->listNode.SetValue(pSlot);

		pSlot->pClipInstance = pAllocator->AllocType<ClipInstance>();

		pSlot->pRampInstance = pAllocator->AllocType<RampInstance>();
		pSlot->pRampInstance->CreateStructure(CLIP_PROPERTY_TYPE_COUNT, pAllocator);

		m_dormantSlotList.AddNode(&pSlot->listNode);
	}
}

void ClipCoordinator::AcquireAudioResources(audi::Device* pAudioDevice)
{
	for (u32 slotIndex = 0; slotIndex < m_slotCapacity; ++slotIndex)
	{
		Slot* pSlot = m_pSlotArray + slotIndex;
		pSlot->pVoiceResource = pAudioDevice->UseVoice();
	}
}

void ClipCoordinator::ReleaseAudioResources(audi::Device* pAudioDevice)
{
	for (u32 slotIndex = 0; slotIndex < m_slotCapacity; ++slotIndex)
	{
		Slot* pSlot = m_pSlotArray + slotIndex;
		pAudioDevice->ReturnVoice(pSlot->pVoiceResource);
	}
}

void ClipCoordinator::StartClip(const ClipAsset* pClipAsset, const RampAsset* pRampAsset)
{
	Slot* pSlot = ActivateSlot();
	if (pSlot == NULL)
	{
		// TODO: kill oldest by priority?
		return;
	}

	ClipInstance* pClipInstance = pSlot->pClipInstance;
	RampInstance* pRampInstance = pSlot->pRampInstance;

	pClipInstance->BindClipAsset(pClipAsset);
	pClipInstance->ResetProperties();

	if (pRampAsset != NULL)
	{
		pRampInstance->BindClipInstance(pSlot->pClipInstance);
		pRampInstance->BindRampAsset(pRampAsset);		
		pRampInstance->ResetTracks();
	}

	pSlot->flags.Set(Slot::FLAG_PLAY_PENDING);
}

void ClipCoordinator::UpdateRamps(f32 secondStep)
{
	for (foun::ListIterator<Slot*> slotIterator = m_activeSlotList.Head(); !slotIterator.IsEnd(); slotIterator.Next())
	{
		Slot* pSlot = slotIterator.GetValue();

		pSlot->pRampInstance->UpdateTracks(secondStep);
	}
}

void ClipCoordinator::ApplyClips(audi::Device* pAudioDevice)
{
	for (foun::ListIterator<Slot*> slotIterator = m_activeSlotList.Head(); !slotIterator.IsEnd(); slotIterator.Next())
	{
		Slot* pSlot = slotIterator.GetValue();
		ClipInstance* pClipInstance = pSlot->pClipInstance;
		audi::VoiceResource* pVoiceResource = pSlot->pVoiceResource;
		bool deactivate = false;

		if (pSlot->flags.Test(Slot::FLAG_PLAY_PENDING))
		{
			// TEMP!
			pAudioDevice->PlayVoice(pVoiceResource, pClipInstance->m_pClipAsset->pSegmentArray[0].pWaveAsset->pWaveResource);

			pSlot->flags.Clear(Slot::FLAG_PLAY_PENDING);
		}

		pAudioDevice->SetVoiceVolume(pVoiceResource, pClipInstance->m_propertyArray[CLIP_PROPERTY_TYPE_VOLUME]);
		pAudioDevice->SetVoicePitch(pVoiceResource, pClipInstance->m_propertyArray[CLIP_PROPERTY_TYPE_PITCH]);

		if (pClipInstance->m_pClipAsset->pClipData->loop)
		{
			// TODO: set loop on voice when appropriate
		}
		else
		{
			if (!pAudioDevice->IsVoicePlaying(pVoiceResource))
			{
				deactivate = true;
			}
		}

		if (deactivate)
		{
			DeactivateSlot(pSlot);
		}
	}
}

/*
* Private Instance
*/
ClipCoordinator::Slot* ClipCoordinator::ActivateSlot()
{
	Slot* pSlot = NULL;
	if (m_dormantSlotList.GetNodeCount() > 0)
	{
		pSlot = m_dormantSlotList.Tail().GetValue();
		m_dormantSlotList.RemoveNode(&pSlot->listNode);
		m_activeSlotList.AddNode(&pSlot->listNode);
	}
	return pSlot;
}

void ClipCoordinator::DeactivateSlot(Slot* pSlot)
{
	m_activeSlotList.RemoveNode(&pSlot->listNode);
	m_dormantSlotList.AddNode(&pSlot->listNode);
}

//namespace soun
}
//namespace lct
}
