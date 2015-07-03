#include <foun/foun_keys.h>

#include <string.h>

namespace lct
{
namespace foun
{

/*
 * Public Static
 */
s32 IntegerKey::Compare(const IntegerKey& keyA, const IntegerKey& keyB)
{
	s32 result;
	if (keyA.m_integer > keyB.m_integer)
	{
		result = -1;
	}
	else if (keyA.m_integer < keyB.m_integer)
	{
		result = 1;
	}
	else
	{
		result = 0;
	}
	return result;
}

/*
 * Public Instance
 */
IntegerKey::IntegerKey()
: m_integer(0)
{
}

IntegerKey::IntegerKey(s32 integer)
: m_integer(integer)
{
}

void IntegerKey::SetInteger(s32 integer)
{
	m_integer = integer;
}


/*
 * Public Static
 */
s32 StringKey::Compare(const StringKey& keyA, const StringKey& keyB)
{
	s32 result = strcmp(keyA.m_pString, keyB.m_pString);
	return result;
}

/*
 * Public Instance
 */
StringKey::StringKey()
: m_pString(NULL)
{
}

StringKey::StringKey(const char* pString)
: m_pString(pString)
{
}

void StringKey::SetString(const char* pString)
{
	m_pString = pString;
}

//namespace lct
}
//namespace foun
}
