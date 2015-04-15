#ifndef SPRI_INSTANCE_BUILDER_H
#define SPRI_INSTANCE_BUILDER_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>

namespace lct
{
namespace spri
{

struct FigureAsset;

class FigureInstance;
class AnimationInstance;

class InstanceBuilder
{
public:
	InstanceBuilder();

	void SetAllocator(lct::foun::Allocator* pAllocator);

	FigureInstance* CreateFigureInstance(FigureAsset& figureAsset);
	AnimationInstance* CreateAnimationInstance(u32 trackCount);

private:
	foun::Allocator* m_pAllocator;
};

//namespace spri
}
//namespace lct
}

#endif//SPRI_INSTANCE_BUILDER_H
