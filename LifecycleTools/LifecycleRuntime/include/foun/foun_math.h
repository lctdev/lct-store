#ifndef FOUN_MATH_H
#define FOUN_MATH_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace foun
{

const f64 PI_64 = 3.14159265358979323846;

const f32 PI_32 = (f32)PI_64;
const f32 HALF_PI_32 = (f32)(PI_64 * 0.5);
const f32 TWO_PI_32 = (f32)(PI_64 * 2.0);

f32 RadiansFromDegrees(f32 degrees);

f32 RadiansFromRotations(f32 rotations);

f32 RotationsFromDegrees(f32 degrees);

f32 Sin(f32 angleRadians);

f32 Cos(f32 angleRadians);

f32 Floor(f32 value);

f32 Lerp(f32 x, f32 x0, f32 x1, f32 y0, f32 y1);

//namespace foun
}
//namespace lct
}

#endif//FOUN_MATH_H
