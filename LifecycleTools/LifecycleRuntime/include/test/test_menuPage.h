#ifndef TEST_MENU_PAGE_H
#define TEST_MENU_PAGE_H

#include <foun/foun_list.h>
#include <foun/foun_vector.h>
#include <foun/foun_rect.h>

#include <test/test_menuShared.h>

namespace lct
{
namespace fill
{
	class DrawContext;
}
namespace font
{
	class SymbolWriter;
}

namespace test
{

class Menu;
class MenuItem;

class MenuPage
{
public:
	MenuPage();
	virtual ~MenuPage();

	void SetLabel(const char* pLabel);
	const char* GetLabel() { return m_pLabel;  }
	void SetPosition(const foun::Vector2& position);
	void SetSpacing(f32 spacing);

	void AddItem(MenuItem* pItem);
	void RemoveItem(MenuItem* pItem);
	MenuItem* GetLastItem();
	void Arrange();

	void HandlePress(const foun::RectEdges& pressBounds);
	void HandleRelease(const foun::RectEdges& pressBounds);

	void DrawFill(fill::DrawContext* pDrawContext);
	void WriteFont(font::SymbolWriter* pSymbolWriter);

protected:
	foun::ListNode<MenuPage*> m_listNode;
	Menu* m_pParent;
	MenuRequest* m_pRequest;

	const char* m_pLabel;
	foun::Vector2 m_position;
	f32 m_spacing;

	foun::List<MenuItem*> m_itemList;
	MenuItem* m_pSelectedItem;

	friend class Menu;
};

//namespace test
}
//namespace lct
}

#endif//TEST_MENU_PAGE_H
