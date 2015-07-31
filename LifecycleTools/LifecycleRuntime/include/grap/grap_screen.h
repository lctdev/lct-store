#ifndef GRAP_SCREEN_H
#define GRAP_SCREEN_H

#include <foun/foun_primitives.h>
#include <foun/foun_rect.h>

namespace lct
{
namespace grap
{

class Screen
{
public:
	Screen();

	void SetWidthHeight(f32 width, f32 height);

	const foun::RectCentered& GetRectCentered() { return m_rectCentered; }
	const foun::RectEdges& GetRectEdges() { return m_rectEdges; }

private:
	foun::RectCentered m_rectCentered;
	foun::RectEdges m_rectEdges;
};

//namespace grap
}
//namespace lct
}

#endif//GRAP_SCREEN_H
