#include <foun/foun_rect.h>

namespace lct
{
namespace foun
{

void RectEdgesFromRectCentered(RectEdges* pRectEdges, const RectCentered& rectCentered)
{
	f32 halfWidth = rectCentered.width / 2.0f;
	pRectEdges->left = rectCentered.x - halfWidth;
	pRectEdges->right = rectCentered.x + halfWidth;

	f32 halfHeight = rectCentered.height / 2.0f;
	pRectEdges->bottom = rectCentered.y - halfHeight;
	pRectEdges->top = rectCentered.y + halfHeight;
}

//namespace lct
}
//namespace foun
}
