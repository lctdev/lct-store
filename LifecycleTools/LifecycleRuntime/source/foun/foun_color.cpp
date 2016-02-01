#include <foun/foun_color.h>

#include <math.h>
#include <stdlib.h>

namespace lct
{
namespace foun
{

void FloatColor3Lerp(FloatColor3& outColor, const FloatColor3& color0, const FloatColor3& color1, float ratio)
{
	float inverseRatio = 1.0f - ratio;
	outColor.r = (color0.r * inverseRatio) + (color1.r * ratio);
	outColor.g = (color0.g * inverseRatio) + (color1.g * ratio);
	outColor.b = (color0.b * inverseRatio) + (color1.b * ratio);
}

//namespace foun
}
//namespace lct
}