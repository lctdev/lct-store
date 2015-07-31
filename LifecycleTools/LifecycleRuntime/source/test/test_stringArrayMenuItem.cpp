#include <test/test_stringArrayMenuItem.h>

#include <stdio.h>

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
StringArrayMenuItem::StringArrayMenuItem()
: CycleMenuItem()
, m_pStringArray(NULL)
, m_count(0)
, m_index(0)
, m_pValueString(NULL)
{
}

StringArrayMenuItem::~StringArrayMenuItem()
{
}

void StringArrayMenuItem::SetStringArray(const char** pStringArray)
{
	m_pStringArray = pStringArray;
}

void StringArrayMenuItem::SetCount(u32 count)
{
	m_count = count;
}

void StringArrayMenuItem::SetIndex(s32 index)
{
	m_index = index;

	UpdateValueString();
}

/*
 * Protected Instance
 */
void StringArrayMenuItem::UpdateValueString()
{
	if ((m_pStringArray != NULL) && (m_index < static_cast<s32>(m_count)))
	{
		m_pValueString = m_pStringArray[m_index];
	}
	else
	{
		m_pValueString = "";
	}
}

const char* StringArrayMenuItem::GetValueString()
{
	return m_pValueString;
}

void StringArrayMenuItem::CycleLeft()
{
	m_index -= 1;
	if (m_index < 0)
	{
		m_index = 0;
	}

	UpdateValueString();	
}

void StringArrayMenuItem::CycleRight()
{
	m_index += 1;
	if (m_index >= static_cast<s32>(m_count))
	{
		m_index = m_count - 1;
	}

	UpdateValueString();
}

//namespace test
}
//namespace lct
}
