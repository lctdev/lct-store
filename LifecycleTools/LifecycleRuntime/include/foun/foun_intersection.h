#ifndef FOUN_INTERSECT_H
#define FOUN_INTERSECT_H

#include <foun/foun_primitives.h>
#include <foun/foun_vector.h>
#include <foun/foun_rect.h>

namespace lct
{
namespace foun
{

bool TestIntersection(const Vector2& point, const RectEdges& rect);
bool TestIntersection(const RectEdges& rectA, const RectEdges& rectB);

//namespace foun
}
//namespace lct
}

#endif//FOUN_INTERSECT_H
