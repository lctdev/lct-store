#include <soun/soun_clipCoordinator.h>
#include <soun/soun_clipInstance.h>
#include <soun/soun_rampInstance.h>
#include <soun/soun_assets.h>
#include <soun/soun_data.h>
#include <soun/soun_handles.h>
#include <soun/soun_stats.h>

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
: m_pAudioDevice(NULL)
, m_slotCapacity(0)
, m_pSlotArray(NULL)
, m_dormantSlotList()
, m_activeSlotList()
{
}

void ClipCoordinator::SetAudioDevice(audi::Device* pAudioDevice)
{
	m_pAudioDevice = pAudioDevice;
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

		pSlot->pClipHandle = NULL;

		m_dormantSlotList.AddNode(&pSlot->listNode);
	}
}

void ClipCoordinator::AcquireAudioResources()
{
	for (u32 slotIndex = 0; slotIndex < m_slotCapacity; ++slotIndex)
	{
		Slot* pSlot = m_pSlotArray + slotIndex;
		pSlot->pVoiceResource = m_pAudioDevice->UseVoice();
	}
}

void ClipCoordinator::ReleaseAudioResources()
{
	for (u32 slotIndex = 0; slotIndex < m_slotCapacity; ++slotIndex)
	{
		Slot* pSlot = m_pSlotArray + slotIndex;
		m_pAudioDevice->ReturnVoice(pSlot->pVoiceResource);
	}
}

void ClipCoordinator::BeginClip(ClipHandle* pClipHandle, const ClipAsset* pClipAsset)
{
	if (pClipHandle != NULL)
	{
		Slot* pSlot = GetSlot(pClipHandle);
		if (pSlot != NULL)
		{
			return;
		}
	}

	Slot* pSlot = ActivateSlot(pClipHandle);
	if (pSlot == NULL)
	{
		// TODO: kill oldest by priority?
		return;
	}	

	ClipInstance* pClipInstance = pSlot->pClipInstance;

	pClipInstance->BindClipAsset(pClipAsset);
	pClipInstance->ResetProperties();

	pSlot->flags.Set(Slot::FLAG_PLAY_PENDING);
}

void ClipCoordinator::SetRamp(ClipHandle* pClipHandle, const RampAsset* pRampAsset)
{
	Slot* pSlot = GetSlot(pClipHandle);
	if (pSlot == NULL)
	{
		return;
	}

	ClipInstance* pClipInstance = pSlot->pClipInstance;
	RampInstance* pRampInstance = pSlot->pRampInstance;

	pRampInstance->BindClipInstance(pSlot->pClipInstance);
	pRampInstance->BindRampAsset(pRampAsset);
	pRampInstance->ResetTracks();
}

void ClipCoordinator::EndClip(ClipHandle* pClipHandle)
{
	Slot* pSlot = GetSlot(pClipHandle);
	if (pSlot == NULL)
	{
		return;
	}

	pSlot->flags.Set(Slot::FLAG_STOP_PENDING);
}

void ClipCoordinator::Update(f32 secondStep)
{
	UpdateVoices();
	UpdateRamps(secondStep);
	RecycleSlots();
}

void ClipCoordinator::FillClipStats(ClipStats* pClipStatsArray, u32 length)
{
	for (u32 slotIndex = 0; (slotIndex < m_slotCapacity) && (slotIndex < length); ++slotIndex)
	{
		ClipStats* pClipStats = pClipStatsArray + slotIndex;
		Slot* pSlot = m_pSlotArray + slotIndex;
		ClipInstance* pClipInstance = pSlot->pClipInstance;

		pClipStats->playing = pSlot->flags.Test(Slot::FLAG_PLAYING);
		pClipStats->volume = pClipInstance->m_propertyArray[CLIP_PROPERTY_TYPE_VOLUME];
		pClipStats->pitch = pClipInstance->m_propertyArray[CLIP_PROPERTY_TYPE_PITCH];
	}
}

/*
* Protected Instance
*/
void ClipCoordinator::UpdateVoices()
{
	for (foun::ListIterator<Slot*> slotIterator = m_activeSlotList.Head(); !slotIterator.IsEnd(); slotIterator.Next())
	{
		Slot* pSlot = slotIterator.GetValue();
		ClipInstance* pClipInstance = pSlot->pClipInstance;
		const ClipAsset* pClipAsset = pClipInstance->m_pClipAsset;
		RampInstance* pRampInstance = pSlot->pRampInstance;
		audi::VoiceResource* pVoiceResource = pSlot->pVoiceResource;

		m_pAudioDevice->SetVoiceVolume(pVoiceResource, pClipInstance->m_propertyArray[CLIP_PROPERTY_TYPE_VOLUME]);
		m_pAudioDevice->SetVoicePitch(pVoiceResource, pClipInstance->m_propertyArray[CLIP_PROPERTY_TYPE_PITCH]);

		if (pSlot->flags.Test(Slot::FLAG_STOP_PENDING))
		{
			// stop any clips with the request, unless they are ramping to a finish
			if (pRampInstance == NULL || pRampInstance->IsFinished())
			{
				m_pAudioDevice->SetVoiceLoop(pVoiceResource, false);
				m_pAudioDevice->StopVoice(pVoiceResource);

				pSlot->flags.Clear(Slot::FLAG_STOP_PENDING);
			}
		}
		else if (pSlot->flags.Test(Slot::FLAG_PLAY_PENDING))
		{
			// play any clips with the request and add their waves
			for (u32 waveIndex = 0; waveIndex < pClipAsset->pClipData->segmentCount; ++waveIndex)
			{
				const WaveAsset* pWaveAsset = pClipAsset->pSegmentArray[waveIndex].pWaveAsset;

				m_pAudioDevice->AddWave(pVoiceResource, pWaveAsset->pWaveResource);
			}

			m_pAudioDevice->PlayVoice(pVoiceResource);

			pSlot->flags.Clear(Slot::FLAG_PLAY_PENDING);
			pSlot->flags.Set(Slot::FLAG_PLAYING);
		}
		else if (pSlot->flags.Test(Slot::FLAG_PLAYING))
		{
			if (pClipAsset->pClipData->loop && !pSlot->flags.Test(Slot::FLAG_LOOPING))
			{
				u32 lastSegmentIndex = pClipAsset->pClipData->segmentCount - 1;
				const WaveAsset* pLastWaveAsset = pClipAsset->pSegmentArray[lastSegmentIndex].pWaveAsset;
				if (m_pAudioDevice->IsWavePlaying(pVoiceResource, pLastWaveAsset->pWaveResource))
				{
					// if the clip should be looping and have started the last wave, remove all previous waves and set up the loop
					for (u32 waveIndex = 0; waveIndex < lastSegmentIndex; ++waveIndex)
					{
						const WaveAsset* pWaveAsset = pClipAsset->pSegmentArray[waveIndex].pWaveAsset;

						m_pAudioDevice->RemoveWave(pVoiceResource, pWaveAsset->pWaveResource);
					}

					m_pAudioDevice->SetVoiceLoop(pVoiceResource, true);

					pSlot->flags.Set(Slot::FLAG_LOOPING);
				}
			}

			if (!m_pAudioDevice->IsVoicePlaying(pVoiceResource))
			{
				// clip is no longer active
				m_pAudioDevice->SetVoiceLoop(pVoiceResource, false);
				m_pAudioDevice->StopVoice(pVoiceResource);

				pRampInstance->ClearRampAsset();

				pSlot->flags.Clear(Slot::FLAG_PLAYING);
				pSlot->flags.Clear(Slot::FLAG_LOOPING);
				pSlot->flags.Clear(Slot::FLAG_ACTIVE);
			}
		}		
	}
}

void ClipCoordinator::UpdateRamps(f32 secondStep)
{
	for (foun::ListIterator<Slot*> slotIterator = m_activeSlotList.Head(); !slotIterator.IsEnd(); slotIterator.Next())
	{
		Slot* pSlot = slotIterator.GetValue();

		pSlot->pRampInstance->UpdateTracks(secondStep);
	}
}

void ClipCoordinator::RecycleSlots()
{
	foun::ListIterator<Slot*> slotIterator = m_activeSlotList.Head();
	while (!slotIterator.IsEnd())
	{
		Slot* pSlot = slotIterator.GetValue();
		ClipInstance* pClipInstance = pSlot->pClipInstance;
		audi::VoiceResource* pVoiceResource = pSlot->pVoiceResource;
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

ClipCoordinator::Slot* ClipCoordinator::ActivateSlot(ClipHandle* pClipHandle)
{
	Slot* pSlot = NULL;
	if (m_dormantSlotList.GetNodeCount() > 0)
	{
		pSlot = m_dormantSlotList.Tail().GetValue();
		m_dormantSlotList.RemoveNode(&pSlot->listNode);
		m_activeSlotList.AddNode(&pSlot->listNode);
	}

	if (pClipHandle != NULL)
	{
		pClipHandle->m_slotIndex = pSlot - m_pSlotArray;

		pSlot->pClipHandle = pClipHandle;
	}

	pSlot->flags.Set(Slot::FLAG_ACTIVE);

	return pSlot;
}

void ClipCoordinator::DeactivateSlot(Slot* pSlot)
{
	ClipHandle* pClipHandle = pSlot->pClipHandle;

	if (pClipHandle != NULL)
	{
		pClipHandle->m_slotIndex = -1;

		pSlot->pClipHandle = NULL;
	}

	pSlot->flags.Clear(Slot::FLAG_ACTIVE);

	m_activeSlotList.RemoveNode(&pSlot->listNode);
	m_dormantSlotList.AddNode(&pSlot->listNode);
}

ClipCoordinator::Slot* ClipCoordinator::GetSlot(ClipHandle* pClipHandle)
{
	if (pClipHandle->m_slotIndex == -1)
	{
		return NULL;
	}
	else
	{
		Slot* pSlot = m_pSlotArray + pClipHandle->m_slotIndex;
		if (pSlot->pClipHandle == pClipHandle)
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
