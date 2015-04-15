#ifndef SPRI_BINDINGS_H
#define SPRI_BINDINGS_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace spri
{

struct FigureAsset;
struct SkinAsset;
struct AnimationAsset;

struct FigureSkinBinding
{
	FigureAsset* pFigureAsset;
	SkinAsset* pSkinAsset;

	s32* pPatternIndexArray;
};

struct AnimationFigureBinding
{
	AnimationAsset* pAnimationAsset;
	FigureAsset* pFigureAsset;

	s32* pTargetIndexArray;
};

//namespace spri
}
//namespace lct
}

#endif//SPRI_BINDINGS_H
