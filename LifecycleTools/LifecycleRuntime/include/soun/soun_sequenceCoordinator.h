#ifndef SOUN_SEQUENCE_COORDINATOR_H
#define SOUN_SEQUENCE_COORDINATOR_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>
#include <foun/foun_list.h>
#include <foun/foun_flags.h>

namespace lct
{
namespace soun
{

class ClipCoordinator;
class SequenceInstance;
class SequenceHandle;
struct SequenceAsset;

class SequenceCoordinator
{
public:
	SequenceCoordinator();

	void SetClipCoordinator(ClipCoordinator* pClipCoordinator);

	void CreateStructure(u32 slotCapacity, foun::Allocator* pAllocator);

	void BeginSequence(SequenceHandle* pSequenceHandle, const SequenceAsset* pSequenceAsset);
	void EndSequence(SequenceHandle* pSequenceHandle);
	void DetachHandle(SequenceHandle* pSequenceHandle);

	void Update(f32 secondStep);

	u32 GetActiveSequenceCount() { return m_activeSlotList.GetNodeCount(); }

protected:
	void UpdateSequences(f32 secondStep);
	void RecycleSlots();

	struct Slot
	{
		foun::ListNode<Slot*> listNode;

		SequenceInstance* pSequenceInstance;

		enum Flag
		{
			FLAG_ACTIVE = 0,
			FLAG_RUNNING,
			FLAG_BREAK_PENDING
		};
		foun::Flags<u32> flags;

		SequenceHandle* pSequenceHandle;
	};

	Slot* ActivateSlot(SequenceHandle* pSequenceHandle);
	void DeactivateSlot(Slot* pSlot);
	Slot* GetSlot(SequenceHandle* pSequenceHandle);

	ClipCoordinator* m_pClipCoordinator;

	u32 m_slotCapacity;
	Slot* m_pSlotArray;
	foun::List<Slot*> m_dormantSlotList;
	foun::List<Slot*> m_activeSlotList;
};

//namespace soun
}
//namespace lct
}

#endif//SOUN_SEQUENCE_COORDINATOR_H
