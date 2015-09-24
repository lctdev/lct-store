#ifndef SOUN_CLIP_INSTANCE_H
#define SOUN_CLIP_INSTANCE_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>
#include <foun/foun_debug.h>

#include <soun/soun_constants.h>

namespace lct
{
namespace audi
{
	struct WaveResource;
}

namespace soun
{

struct ClipAsset;
struct SegmentData;
struct ClipData;

class ClipInstance
{
public:
	ClipInstance();

	void BindClipAsset(const ClipAsset* pClipAsset);

	void ResetProperties();

protected:
	const ClipAsset* m_pClipAsset;

	f32 m_propertyArray[CLIP_PROPERTY_TYPE_COUNT];

	friend class RampInstance;
	friend class ClipCoordinator;
};

//namespace soun
}
//namespace lct
}

#endif//SOUN_CLIP_INSTANCE_H
