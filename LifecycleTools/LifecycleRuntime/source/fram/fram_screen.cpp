#include <fram/fram_screen.h>

namespace lct
{
namespace fram
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

	lct::foun::RectEdgesFromRectCentered(&m_rectEdges, m_rectCentered);
}

//namespace fram
}
//namespace lct
}
