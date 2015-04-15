#include <foun/foun_intersection.h>
#include <foun/foun_math.h>

namespace lct
{
namespace foun
{

bool TestIntersection(const Vector2& point, const RectEdges& rect)
{
	bool intersect = (point.x >= rect.left) && (point.x <= rect.right) && (point.y >= rect.bottom) && (point.y <= rect.top);
	return intersect;
}

//namespace foun
}
//namespace lct
}
