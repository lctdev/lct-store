#ifndef FOUN_COLOR_H
#define FOUN_COLOR_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace foun
{

struct FloatColor3
{
	f32 r;
	f32 g;
	f32 b;
};

struct FloatColor4
{
	f32 r;
	f32 g;
	f32 b;
	f32 a;
};

void FloatColor3Lerp(FloatColor3& outColor, const FloatColor3& color0, const FloatColor3& color1, float ratio);

//namespace foun
}
//namespace lct
}

#endif//FOUN_COLOR_H
