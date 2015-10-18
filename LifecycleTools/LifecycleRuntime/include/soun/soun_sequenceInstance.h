#ifndef SOUN_SEQUENCE_INSTANCE_H
#define SOUN_SEQUENCE_INSTANCE_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>

namespace lct
{
namespace soun
{

class ClipCoordinator;
class ClipHandle;

struct SequenceAsset;

class SequenceInstance
{
public:
	SequenceInstance();

	void SetClipCoordinator(ClipCoordinator* pClipCoordinator);

	void CreateStructure(u32 clipHandleCapacity, foun::Allocator* pAllocator);	

	void ClearSequenceAsset();
	void BindSequenceAsset(const SequenceAsset* pSequenceAsset);

	f32 GetTimelineSecondDuration() { return m_timeline.secondDuration;  }
	bool IsFinished();

	void SetTimelineIndex(u32 timelineIndex);
	void ResetTimeline();
	void UpdateTimeline(f32 secondStep);

protected:
	ClipCoordinator* m_pClipCoordinator;
	u32 m_clipHandleCapacity;
	ClipHandle* m_pClipHandleArray;

	const SequenceAsset* m_pSequenceAsset;	

	struct Timeline
	{
		u32 actionIndex;
		f32 second;
		f32 secondDuration;
	};	
	Timeline m_timeline;
	u32 m_timelineIndex;

	friend class SequenceCoordinator;
};

//namespace soun
}
//namespace lct
}

#endif//SOUN_SEQUENCE_INSTANCE_H
