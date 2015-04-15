#ifndef FRAM_SCREEN_H
#define FRAM_SCREEN_H

#include <foun/foun_primitives.h>
#include <foun/foun_rect.h>

namespace lct
{
namespace fram
{

class Screen
{
public:
	Screen();

	void SetWidthHeight(f32 width, f32 height);

	const lct::foun::RectCentered& GetRectCentered() { return m_rectCentered; }
	const lct::foun::RectEdges& GetRectEdges() { return m_rectEdges; }

private:
	lct::foun::RectCentered m_rectCentered;
	lct::foun::RectEdges m_rectEdges;
};

//namespace fram
}
//namespace lct
}

#endif//FRAM_SCREEN_H
