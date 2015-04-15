#ifndef TEST_BOOL_MENU_ITEM_H
#define TEST_BOOL_MENU_ITEM_H

#include <test/test_cycleMenuItem.h>

namespace lct
{
namespace test
{

class BoolMenuItem : public CycleMenuItem
{
public:
	BoolMenuItem();
	virtual ~BoolMenuItem();

	void SetValue(bool value);

	bool GetValue() { return m_value; }

	void SetCallback(foun::Callback<bool, int>* pCallback);

protected:
	bool m_value;

	void UpdateValueString();

	static const u32 VALUE_STRING_SIZE = 32;
	char m_valueString[VALUE_STRING_SIZE];

	foun::Callback<bool, int>* m_pCallback;

	virtual const char* GetValueString();

	virtual void CycleLeft();
	virtual void CycleRight();
};

//namespace test
}
//namespace lct
}

#endif//TEST_BOOL_MENU_ITEM_H
