#include <inpu/inpu_touchCursor.h>
#include <inpu/inpu_touchReader.h>

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
TouchCursor::TouchCursor()
: Cursor()
, m_pReader(NULL)
{
}

TouchCursor::~TouchCursor()
{
}

void TouchCursor::SetReader(TouchReader* pReader)
{
	m_pReader = pReader;
}

s16 TouchCursor::GetX()
{
	return m_pReader->GetX();
}

s16 TouchCursor::GetY()
{
	return m_pReader->GetY();
}

bool TouchCursor::IsDown()
{
	return m_pReader->IsDown();
}

bool TouchCursor::IsPress()
{
	return m_pReader->IsPress();
}

bool TouchCursor::IsRelease()
{
	return m_pReader->IsRelease();
}

//namespace inpu
}
//namespace lct
}
