#ifndef SPRI_ANIMATION_INSTANCE_H
#define SPRI_ANIMATION_INSTANCE_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace spri
{

class FigureInstance;

struct AnimationAsset;
struct KeyData;
struct TrackData;
struct AnimationData;

struct AnimationFigureBinding;

class AnimationInstance
{
public:
	AnimationInstance();

	void BindFigureInstance(FigureInstance& figureInstance);
	void ClearAnimationAsset();
	void BindAnimationAsset(AnimationAsset& animationAsset, AnimationFigureBinding& animationFigureBinding);

	f32 FindLongestTrackFrameDuration();

	void ResetTracks();
	void UpdateTracks(f32 frameStep);
	void ForceFrameOnTracks(f32 frame);

protected:
	FigureInstance* m_pFigureInstance;

	AnimationAsset* m_pAnimationAsset;

	struct Track
	{
		const TrackData* pTrackData;
		const KeyData* pKeyDataArray;

		u32 keyIndex;
		f32 frame;
		f32* pTargetProperty;
		f32 frameDuration;
	};
	Track* m_pTrackArray;
	u32 m_trackCount;
	u32 m_activeTrackCount;

	friend class InstanceBuilder;
};

//namespace spri
}
//namespace lct
}

#endif//SPRI_ANIMATION_INSTANCE_H
