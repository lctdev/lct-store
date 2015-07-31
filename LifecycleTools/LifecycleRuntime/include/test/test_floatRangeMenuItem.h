#ifndef TEST_FLOAT_RANGE_MENU_ITEM_H
#define TEST_FLOAT_RANGE_MENU_ITEM_H

#include <test/test_cycleMenuItem.h>

namespace lct
{
namespace test
{

class FloatRangeMenuItem : public CycleMenuItem
{
public:
	FloatRangeMenuItem();
	virtual ~FloatRangeMenuItem();

	void SetWrap(bool wrap);
	void SetMin(f32 min);
	void SetMax(f32 max);
	void SetStep(f32 step);
	void SetValue(f32 value);

	f32 GetMax() { return m_max; }
	f32 GetValue() { return m_value; }

protected:
	bool m_wrap;
	f32 m_min;
	f32 m_max;
	f32 m_step;
	f32 m_value;

	void UpdateValueString();

	static const u32 VALUE_STRING_SIZE = 32;
	char m_valueString[VALUE_STRING_SIZE];

	virtual const char* GetValueString();

	virtual void CycleLeft();
	virtual void CycleRight();
};

//namespace test
}
//namespace lct
}

#endif//TEST_FLOAT_RANGE_MENU_ITEM_H
