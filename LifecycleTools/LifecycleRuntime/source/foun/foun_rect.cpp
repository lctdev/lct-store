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

void RectCenteredFromRectEdges(RectCentered* pRectCentered, const RectEdges& rectEdges)
{
	f32 width = rectEdges.right - rectEdges.left;
	pRectCentered->width = width;
	pRectCentered->x = rectEdges.left + (width / 2.0f);

	f32 height = rectEdges.top - rectEdges.bottom;
	pRectCentered->height = height;
	pRectCentered->y = rectEdges.bottom + (height / 2.0f);
}

//namespace lct
}
//namespace foun
}
