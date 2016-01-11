#include <foun/foun_vector.h>
#include <foun/foun_math.h>

namespace lct
{
namespace foun
{

void Vector2Add(Vector2& vector, const Vector2& vectorA, const Vector2& vectorB)
{
	vector.x = vectorA.x + vectorB.x;
	vector.y = vectorA.y + vectorB.y;
}

void Vector2Subtract(Vector2& vector, const Vector2& vectorA, const Vector2& vectorB)
{
	vector.x = vectorA.x - vectorB.x;
	vector.y = vectorA.y - vectorB.y;
}

f32 Vector2LengthSquared(const Vector2& vector)
{
	f32 lengthSquared = (vector.x * vector.x) + (vector.y * vector.y);
	return lengthSquared;
}

f32 Vector2Length(const Vector2& vector)
{
	f32 lengthSquared = (vector.x * vector.x) + (vector.y * vector.y);
	return lct::foun::Sqrt(lengthSquared);
}

f32 Vector2DistanceSquared(const Vector2& vectorA, const Vector2& vectorB)
{
	Vector2 vector;
	vector.x = vectorA.x - vectorB.x;
	vector.y = vectorA.y - vectorB.y;
	f32 distanceSquared = (vector.x * vector.x) + (vector.y * vector.y);
	return distanceSquared;
}

f32 Vector2Distance(const Vector2& vectorA, const Vector2& vectorB)
{
	Vector2 vector;
	vector.x = vectorA.x - vectorB.x;
	vector.y = vectorA.y - vectorB.y;
	f32 distanceSquared = (vector.x * vector.x) + (vector.y * vector.y);
	return lct::foun::Sqrt(distanceSquared);
}

//namespace foun
}
//namespace lct
}
