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
struct ClipAsset;
struct RampAsset;

class ClipCoordinator
{
public:
	ClipCoordinator();

	void CreateStructure(u32 slotCapacity, foun::Allocator* pAllocator);
	void AcquireAudioResources(audi::Device* pAudioDevice);
	void ReleaseAudioResources(audi::Device* pAudioDevice);

	void StartClip(const ClipAsset* pClipAsset, const RampAsset* pRampAsset);
	void UpdateRamps(f32 secondStep);
	void ApplyClips(audi::Device* pAudioDevice);

	u32 GetActiveCount() { return m_activeSlotList.GetNodeCount(); }

protected:
	struct Slot
	{
		foun::ListNode<Slot*> listNode;

		ClipInstance* pClipInstance;
		RampInstance* pRampInstance;

		enum Flag
		{
			FLAG_PLAY_PENDING = 0
		};
		foun::Flags<u32> flags;

		audi::VoiceResource* pVoiceResource;		
	};

	Slot* ActivateSlot();
	void DeactivateSlot(Slot* pSlot);

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
