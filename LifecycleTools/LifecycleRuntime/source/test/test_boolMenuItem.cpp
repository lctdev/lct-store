#include <test/test_boolMenuItem.h>

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
BoolMenuItem::BoolMenuItem()
: CycleMenuItem()
, m_value(false)
, m_valueString()
, m_pCallback(NULL)
{
}

BoolMenuItem::~BoolMenuItem()
{
}

void BoolMenuItem::SetValue(bool value)
{
	m_value = value;

	UpdateValueString();
}

void BoolMenuItem::SetCallback(foun::Callback<bool, int>* pCallback)
{
	m_pCallback = pCallback;
}

/*
 * Protected Instance
 */
void BoolMenuItem::UpdateValueString()
{
	if (m_value)
	{
		//snprintf(m_valueString, VALUE_STRING_SIZE, "true");
		LCT_SPRINTF(m_valueString, VALUE_STRING_SIZE, "true");
	}
	else
	{
		LCT_SPRINTF(m_valueString, VALUE_STRING_SIZE, "false");
	}
}

const char* BoolMenuItem::GetValueString()
{
	return m_valueString;
}

void BoolMenuItem::CycleLeft()
{
	m_value = false;

	UpdateValueString();

	if (m_pCallback != NULL)
	{
		m_pCallback->Invoke(m_value);
	}
}

void BoolMenuItem::CycleRight()
{
	m_value = true;

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
