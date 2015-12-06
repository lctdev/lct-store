#include <inpu/inpu_touchReader.h>

#include <foun/foun_platform.h>

#if defined(LCT_IOS)
#include <inpu/inpu_touchReader_OBJC.h>
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
TouchReader::TouchReader()
: m_x(0)
, m_y(0)
, m_prevDown(false)
, m_currDown(false)
{
}

void TouchReader::PrepareValues()
{
	m_prevDown = m_currDown;
}

bool TouchReader::HandlePlatformMessage(const foun::PlatformMessage& platformMessage)
{
#if defined(LCT_ANDROID)
	AInputEvent* event = platformMessage.inputEvent;
	if (event != NULL)
	{
		int32_t inputType = AInputEvent_getType(event);
		if (inputType == AINPUT_EVENT_TYPE_MOTION)
		{
			int32_t actionType = AMotionEvent_getAction(event);
			switch (actionType)
			{
			case AMOTION_EVENT_ACTION_DOWN:
			{
				m_x = AMotionEvent_getX(event, 0);
				m_y = AMotionEvent_getY(event, 0);
				m_currDown = true;

				return true;
			}
			break;

			case AMOTION_EVENT_ACTION_UP:
			{
				m_x = AMotionEvent_getX(event, 0);
				m_y = AMotionEvent_getY(event, 0);
				m_currDown = false;

				return true;
			}
			break;

			case AMOTION_EVENT_ACTION_MOVE:
			{
				m_x = AMotionEvent_getX(event, 0);
				m_y = AMotionEvent_getY(event, 0);

				return true;
			}
			break;
			}
		}
	}
#elif defined(LCT_IOS)
	//LogEvent(platformMessage.uiEvent);

	if (IsTouchDownEvent(platformMessage.uiEvent))
	{
		m_x = GetXParam(platformMessage.uiEvent);
		m_y = GetYParam(platformMessage.uiEvent);
		m_currDown = true;
		
		return true;
	}
	
	if (IsTouchUpEvent(platformMessage.uiEvent))
	{
		m_x = GetXParam(platformMessage.uiEvent);
		m_y = GetYParam(platformMessage.uiEvent);
		m_currDown = false;
		
		return true;
	}

	if (IsTouchMoveEvent(platformMessage.uiEvent))
	{
		m_x = GetXParam(platformMessage.uiEvent);
		m_y = GetYParam(platformMessage.uiEvent);
	
		return true;
	}
#endif
	return false;
}

bool TouchReader::IsDown()
{
	return m_currDown;
}

bool TouchReader::IsPress()
{
	return (!m_prevDown && m_currDown);
}

bool TouchReader::IsRelease()
{
	return (m_prevDown && !m_currDown);
}

//namespace inpu
}
//namespace lct
}
