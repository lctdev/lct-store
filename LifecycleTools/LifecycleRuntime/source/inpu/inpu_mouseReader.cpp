#include <inpu/inpu_mouseReader.h>
#include <inpu/inpu_mouseReader_OBJC.h>

#include <foun/foun_platform.h>

#if defined(LCT_WINDOWS)
#include <windowsx.h>
#endif

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
MouseReader::MouseReader()
: m_x(0)
, m_y(0)
, m_prevButtonDownFlags()
, m_currButtonDownFlags()
{
}

void MouseReader::PrepareValues()
{
	m_prevButtonDownFlags.SetValues(m_currButtonDownFlags.GetValues());
}

bool MouseReader::HandlePlatformMessage(const foun::PlatformMessage& platformMessage)
{
#if defined(LCT_WINDOWS)
	switch (platformMessage.message)
	{
	case WM_MOUSEMOVE:
	{
		m_x = GET_X_LPARAM(platformMessage.lParam);
		m_y = GET_Y_LPARAM(platformMessage.lParam);

		return true;
	}
	break;

	case WM_LBUTTONDOWN:
	{
		m_currButtonDownFlags.Set(MOUSE_BUTTON_LEFT);

		return true;
	}
	break;

	case WM_LBUTTONUP:
	{
		m_currButtonDownFlags.Clear(MOUSE_BUTTON_LEFT);

		return true;
	}
	break;

	default:
	{
		return false;
	}
	}
#elif defined(LCT_OSX)
    if (IsMouseDownEvent(platformMessage.nsEvent))
	{
		m_x = GetXParam(platformMessage.nsEvent);
		m_y = GetYParam(platformMessage.nsEvent);
		m_currButtonDownFlags.Set(MOUSE_BUTTON_LEFT);
		
		return true;
	}
	
	if (IsMouseUpEvent(platformMessage.nsEvent))
	{
		m_x = GetXParam(platformMessage.nsEvent);
		m_y = GetYParam(platformMessage.nsEvent);
		m_currButtonDownFlags.Clear(MOUSE_BUTTON_LEFT);
		
		return true;
	}
	
	if (IsMouseMoveEvent(platformMessage.nsEvent))
	{
		m_x = GetXParam(platformMessage.nsEvent);
		m_y = GetYParam(platformMessage.nsEvent);
	
		return true;
	}

	return false;
#endif
}

bool MouseReader::IsButtonDown(MouseButton button)
{
	bool isDown = m_currButtonDownFlags.Test(button);
	return isDown;
}

bool MouseReader::IsButtonPress(MouseButton button)
{
	bool wasDown = m_prevButtonDownFlags.Test(button);
	bool isDown = m_currButtonDownFlags.Test(button);
	return (!wasDown && isDown);
}

bool MouseReader::IsButtonRelease(MouseButton button)
{
	bool wasDown = m_prevButtonDownFlags.Test(button);
	bool isDown = m_currButtonDownFlags.Test(button);
	return (wasDown && !isDown);
}

//namespace inpu
}
//namespace lct
}
