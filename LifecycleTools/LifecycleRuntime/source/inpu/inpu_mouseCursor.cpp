#include <inpu/inpu_mouseCursor.h>
#include <inpu/inpu_mouseReader.h>

namespace lct
{
namespace inpu
{

/*
 * Internal Constants
 */

/*
 * Public Instance
 */
MouseCursor::MouseCursor()
: Cursor()
, m_pReader(NULL)
{
}

MouseCursor::~MouseCursor()
{
}

void MouseCursor::SetReader(MouseReader* pReader)
{
	m_pReader = pReader;
}

s16 MouseCursor::GetX()
{
	return m_pReader->GetX();
}

s16 MouseCursor::GetY()
{
	return m_pReader->GetY();
}

bool MouseCursor::IsDown()
{
	return m_pReader->IsButtonDown(MOUSE_BUTTON_LEFT);
}

bool MouseCursor::IsPress()
{
	return m_pReader->IsButtonPress(MOUSE_BUTTON_LEFT);
}

bool MouseCursor::IsRelease()
{
	return m_pReader->IsButtonRelease(MOUSE_BUTTON_LEFT);
}

//namespace inpu
}
//namespace lct
}
