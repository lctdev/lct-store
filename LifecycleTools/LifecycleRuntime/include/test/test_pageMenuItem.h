#ifndef TEST_PAGE_MENU_ITEM_H
#define TEST_PAGE_MENU_ITEM_H

#include <foun/foun_primitives.h>
#include <foun/foun_color.h>
#include <foun/foun_callback.h>

#include <test/test_menuItem.h>
#include <test/test_button.h>

namespace lct
{
namespace test
{

class PageMenuItem : public MenuItem
{
public:
	PageMenuItem();
	virtual ~PageMenuItem();

	void SetOffsets(f32 buttonOffsetX);

	virtual void Arrange();

	virtual void SetIsSelected(bool isSelected);
	virtual void HandlePress(const foun::RectEdges& pressBounds);
	virtual void HandleRelease(const foun::RectEdges& pressBounds);

	virtual void DrawFill(fill::DrawContext* pDrawContext);
	virtual void WriteFont(font::SymbolWriter* pSymbolWriter);

protected:
	void OnTrigger();

	Button m_button;

	f32 m_buttonOffsetX;
};

//namespace test
}
//namespace lct
}

#endif//TEST_PAGE_MENU_ITEM_H
