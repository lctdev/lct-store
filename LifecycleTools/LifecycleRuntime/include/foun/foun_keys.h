#ifndef FOUN_KEYS_H
#define FOUN_KEYS_H

#include <foun/foun_primitives.h>

namespace lct
{
namespace foun
{

class IntegerKey
{
public:
	static s32 Compare(const IntegerKey& keyA, const IntegerKey& keyB);

public:
	IntegerKey();

	void SetInteger(s32 integer);

private:
	s32 m_integer;
};

class StringKey
{
public:
	static s32 Compare(const StringKey& keyA, const StringKey& keyB);

public:
	StringKey();

	void SetString(const char* pString);
	const char* GetString() { return m_pString; }

private:
	const char* m_pString;
};

//namespace lct
}
//namespace foun
}

#endif//FOUN_KEYS_H
