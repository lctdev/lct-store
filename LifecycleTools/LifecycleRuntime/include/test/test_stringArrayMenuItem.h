#ifndef TEST_STRING_ARRAY_MENU_ITEM_H
#define TEST_STRING_ARRAY_MENU_ITEM_H

#include <test/test_cycleMenuItem.h>

namespace lct
{
namespace test
{

class StringArrayMenuItem : public CycleMenuItem
{
public:
	StringArrayMenuItem();
	virtual ~StringArrayMenuItem();

	void SetStringArray(const char** pStringArray);
	void SetCount(u32 count);
	void SetIndex(s32 index);

	s32 GetIndex() { return m_index; }	

protected:
	const char** m_pStringArray;
	u32 m_count;
	s32 m_index;

	void UpdateValueString();

	const char* m_pValueString;	

	virtual const char* GetValueString();

	virtual void CycleLeft();
	virtual void CycleRight();
};

//namespace test
}
//namespace lct
}

#endif//TEST_STRING_ARRAY_MENU_ITEM_H
