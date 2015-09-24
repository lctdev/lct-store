#ifndef SOUN_RAMP_INSTANCE_H
#define SOUN_RAMP_INSTANCE_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>

namespace lct
{
namespace soun
{

class ClipInstance;

struct RampAsset;
struct KeyData;
struct TrackData;
struct RampData;

class RampInstance
{
public:
	RampInstance();

	void CreateStructure(u32 trackCapacity, foun::Allocator* pAllocator);

	void BindClipInstance(ClipInstance* pClipInstance);
	void ClearRampAsset();
	void BindRampAsset(const RampAsset* pRampAsset);

	f32 FindLongestTrackSecondDuration();

	void ResetTracks();
	void UpdateTracks(f32 secondStep);
	void ForceSecondOnTracks(f32 second);

protected:
	ClipInstance* m_pClipInstance;

	const RampAsset* m_pRampAsset;

	struct Track
	{
		u32 keyIndex;
		f32 second;
		f32* pTargetProperty;
		f32 secondDuration;
	};
	u32 m_trackCapacity;
	u32 m_trackCount;
	Track* m_pTrackArray;	
};

//namespace soun
}
//namespace lct
}

#endif//SOUN_RAMP_INSTANCE_H
