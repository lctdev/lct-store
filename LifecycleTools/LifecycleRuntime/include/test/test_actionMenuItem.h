#ifndef TEST_ACTION_MENU_ITEM_H
#define TEST_ACTION_MENU_ITEM_H

#include <foun/foun_primitives.h>
#include <foun/foun_color.h>
#include <foun/foun_callback.h>

#include <test/test_menuItem.h>
#include <test/test_button.h>

namespace lct
{
namespace test
{

class ActionMenuItem : public MenuItem
{
public:
	ActionMenuItem();
	virtual ~ActionMenuItem();

	void SetOffsets(f32 buttonOffsetX);
	foun::Callback& GetCallback() { return m_button.GetTriggerCallback(); }

	virtual void Arrange();

	virtual void SetIsSelected(bool isSelected);
	virtual void HandlePress(const foun::Vector2& position);
	virtual void HandleRelease(const foun::Vector2& position);

	virtual void DrawFill(fill::DrawContext* pDrawContext);
	virtual void WriteFont(font::SymbolWriter* pSymbolWriter);

protected:
	Button m_button;

	f32 m_buttonOffsetX;
};

//namespace test
}
//namespace lct
}

#endif//TEST_ACTION_MENU_ITEM_H
