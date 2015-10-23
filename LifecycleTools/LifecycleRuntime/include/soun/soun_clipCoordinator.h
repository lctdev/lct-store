#ifndef SOUN_CLIP_COORDINATOR_H
#define SOUN_CLIP_COORDINATOR_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>
#include <foun/foun_list.h>
#include <foun/foun_flags.h>

namespace lct
{
namespace audi
{
class Device;
struct VoiceResource;
}

namespace soun
{

class ClipInstance;
class RampInstance;
class ClipHandle;
struct ClipAsset;
struct RampAsset;
struct ClipStats;

class ClipCoordinator
{
public:
	ClipCoordinator();

	void SetAudioDevice(audi::Device* pAudioDevice);

	void CreateStructure(u32 slotCapacity, foun::Allocator* pAllocator);
	void AcquireAudioResources();
	void ReleaseAudioResources();

	void BeginClip(ClipHandle* pClipHandle, const ClipAsset* pClipAsset);
	void EndClip(ClipHandle* pClipHandle);
	void SetRamp(ClipHandle* pClipHandle, const RampAsset* pRampAsset);
	void ClearRamp(ClipHandle* pClipHandle);
	void DetachHandle(ClipHandle* pClipHandle);

	void Update(f32 secondStep);

	u32 GetActiveClipCount() { return m_activeSlotList.GetNodeCount(); }
	void FillClipStats(ClipStats* pClipStatsArray, u32 length);

protected:
	void UpdateVoices();
	void UpdateRamps(f32 secondStep);
	void RecycleSlots();	

	struct Slot
	{
		foun::ListNode<Slot*> listNode;

		ClipInstance* pClipInstance;
		RampInstance* pRampInstance;

		enum Flag
		{
			FLAG_ACTIVE = 0,
			FLAG_PLAY_PENDING,
			FLAG_PLAYING,
			FLAG_LOOPING,
			FLAG_STOP_PENDING
		};
		foun::Flags<u32> flags;

		audi::VoiceResource* pVoiceResource;

		ClipHandle* pClipHandle;
	};

	Slot* ActivateSlot(ClipHandle* pClipHandle);
	void DeactivateSlot(Slot* pSlot);
	Slot* GetSlot(ClipHandle* pClipHandle);

	audi::Device* m_pAudioDevice;

	u32 m_slotCapacity;
	Slot* m_pSlotArray;
	foun::List<Slot*> m_dormantSlotList;
	foun::List<Slot*> m_activeSlotList;
};

//namespace soun
}
//namespace lct
}

#endif//SOUN_CLIP_COORDINATOR_H
