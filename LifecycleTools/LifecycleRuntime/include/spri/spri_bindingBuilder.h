#ifndef SPRI_BINDING_BUILDER_H
#define SPRI_BINDING_BUILDER_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>

namespace lct
{
namespace spri
{

struct FigureAsset;
struct SkinAsset;
struct AnimationAsset;

struct FigureSkinBinding;
struct AnimationFigureBinding;

class BindingBuilder
{
public:
	BindingBuilder();

	void SetAllocator(lct::foun::Allocator* pAllocator);

	FigureSkinBinding* CreateFigureSkinBinding(FigureAsset& figureAsset, SkinAsset& skinAsset);
	AnimationFigureBinding* CreateAnimationFigureBinding(AnimationAsset& animationAsset, FigureAsset& figureAsset);

private:
	foun::Allocator* m_pAllocator;
};

//namespace spri
}
//namespace lct
}

#endif//SPRI_BINDING_BUILDER_H
