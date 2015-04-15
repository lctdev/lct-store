#ifndef FOUN_RECT_H
#define FOUN_RECT_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace foun
{

struct RectCentered
{
	f32 x;
	f32 y;
	f32 width;
	f32 height;
};

struct RectEdges
{
	f32 left;
	f32 right;
	f32 bottom;
	f32 top;
};

void RectEdgesFromRectCentered(RectEdges* pRectEdges, const RectCentered& rectCentered);

//namespace foun
}
//namespace lct
}

#endif//FOUN_RECT_H
