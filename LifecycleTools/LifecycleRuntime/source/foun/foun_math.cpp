#include <foun/foun_math.h>

#include <math.h>

namespace lct
{
namespace foun
{

f32 RadiansFromDegrees(f32 degrees)
{
    return (degrees / 180.0f * PI_32);
}

f32 RadiansFromRotations(f32 rotations)
{
    return (rotations * TWO_PI_32);
}

f32 RotationsFromDegrees(f32 degrees)
{
    return (degrees / 360.0f);
}

f32 Sin(f32 angleRadians)
{
    return sin(angleRadians);
}

f32 Cos(f32 angleRadians)
{
    return cos(angleRadians);
}

f32 Floor(f32 value)
{
    return floor(value);
}

//namespace foun
}
//namespace lct
}
