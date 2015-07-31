#ifndef TEST_CYCLE_MENU_ITEM_H
#define TEST_CYCLE_MENU_ITEM_H

#include <foun/foun_primitives.h>
#include <foun/foun_color.h>
#include <foun/foun_callback.h>

#include <test/test_menuItem.h>
#include <test/test_button.h>

namespace lct
{
namespace test
{

class CycleMenuItem : public MenuItem
{
public:
	CycleMenuItem();
	virtual ~CycleMenuItem();

	void SetOffsets(f32 valueOffsetX, f32 leftButtonOffsetX, f32 rightButtonOffsetX);

	foun::Callback& GetCycleCallback() { return m_cycleCallback; }

	virtual void Arrange();

	virtual void SetIsSelected(bool isSelected);
	virtual void HandlePress(const foun::Vector2& position);
	virtual void HandleRelease(const foun::Vector2& position);

	virtual void DrawFill(fill::DrawContext* pDrawContext);
	virtual void WriteFont(font::SymbolWriter* pSymbolWriter);

protected:
	virtual const char* GetValueString() = 0;

	void OnLeftTrigger();
	void OnRightTrigger();

	virtual void CycleLeft() = 0;
	virtual void CycleRight() = 0;

	Button m_leftButton;
	Button m_rightButton;

	f32 m_valueOffsetX;
	f32 m_leftButtonOffsetX;
	f32 m_rightButtonoffsetX;

	foun::Callback m_cycleCallback;
};

//namespace test
}
//namespace lct
}

#endif//TEST_CYCLE_MENU_ITEM_H
