#include <grap/grap_screen.h>

namespace lct
{
namespace grap
{

/*
 * Public Instance
 */
Screen::Screen()
: m_rectCentered()
, m_rectEdges()
{
}

void Screen::SetWidthHeight(f32 width, f32 height)
{
	m_rectCentered.x = 0.0f;
	m_rectCentered.y = 0.0f;
	m_rectCentered.width = width;
	m_rectCentered.height = height;

	foun::RectEdgesFromRectCentered(&m_rectEdges, m_rectCentered);
}

//namespace grap
}
//namespace lct
}
