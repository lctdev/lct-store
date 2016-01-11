#ifndef FOUN_VECTOR_H
#define FOUN_VECTOR_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace foun
{

struct Vector2
{
	f32 x;
	f32 y;
};

struct Vector4
{
	f32 x;
	f32 y;
	f32 z;
	f32 w;
};

void Vector2Add(Vector2& vector, const Vector2& vectorA, const Vector2& vectorB);
void Vector2Subtract(Vector2& vector, const Vector2& vectorA, const Vector2& vectorB);
f32 Vector2LengthSquared(const Vector2& vector);
f32 Vector2Length(const Vector2& vector);
f32 Vector2DistanceSquared(const Vector2& vectorA, const Vector2& vectorB);
f32 Vector2Distance(const Vector2& vectorA, const Vector2& vectorB);

//namespace foun
}
//namespace lct
}

#endif//FOUN_VECTOR_H
