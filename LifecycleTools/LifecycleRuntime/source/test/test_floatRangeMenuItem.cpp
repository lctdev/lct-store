#include <test/test_floatRangeMenuItem.h>

#include <foun/foun_string.h>

namespace lct
{
namespace test
{

/*
 * Internal Constants
 */

/*
 * Public Instance
 */
FloatRangeMenuItem::FloatRangeMenuItem()
: CycleMenuItem()
, m_wrap(false)
, m_min(0.0f)
, m_max(0.0f)
, m_step(0.0f)
, m_value(0.0f)
, m_valueString()
, m_pCallback(NULL)
{
}

FloatRangeMenuItem::~FloatRangeMenuItem()
{
}

void FloatRangeMenuItem::SetWrap(bool wrap)
{
	m_wrap = wrap;
}

void FloatRangeMenuItem::SetMin(f32 min)
{
	m_min = min;
}

void FloatRangeMenuItem::SetMax(f32 max)
{
	m_max = max;
}

void FloatRangeMenuItem::SetStep(f32 step)
{
	m_step = step;
}

void FloatRangeMenuItem::SetValue(f32 value)
{
	m_value = value;

	UpdateValueString();
}

void FloatRangeMenuItem::SetCallback(foun::Callback<float, int>* pCallback)
{
	m_pCallback = pCallback;
}

/*
 * Protected Instance
 */
void FloatRangeMenuItem::UpdateValueString()
{
	LCT_SPRINTF(m_valueString, VALUE_STRING_SIZE, "%4.2f", m_value);
}

const char* FloatRangeMenuItem::GetValueString()
{
	return m_valueString;
}

void FloatRangeMenuItem::CycleLeft()
{
	m_value -= m_step;
	if (m_wrap)
	{
		f32 range = m_max - m_min;
		while (m_value < m_min)
		{
			m_value += range;
		}
	}
	else
	{
		if (m_value < m_min)
		{
			m_value = m_min;
		}
	}

	UpdateValueString();

	if (m_pCallback != NULL)
	{
		m_pCallback->Invoke(m_value);
	}
}

void FloatRangeMenuItem::CycleRight()
{
	m_value += m_step;
	if (m_wrap)
	{
		f32 range = m_max - m_min;
		while (m_value > m_max)
		{
			m_value -= range;
		}
	}
	else
	{
		if (m_value > m_max)
		{
			m_value = m_max;
		}
	}

	UpdateValueString();

	if (m_pCallback != NULL)
	{
		m_pCallback->Invoke(m_value);
	}
}

//namespace test
}
//namespace lct
}
