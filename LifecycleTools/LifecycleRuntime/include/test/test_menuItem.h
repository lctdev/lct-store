#ifndef TEST_MENU_ITEM_H
#define TEST_MENU_ITEM_H

#include <foun/foun_primitives.h>
#include <foun/foun_color.h>
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
class MenuPage;

class MenuItem
{
public:
	MenuItem();
	virtual ~MenuItem();

	void SetLabel(const char* pLabel);
	void SetPosition(const foun::Vector2& position);

	virtual void Arrange();

	virtual void SetIsSelected(bool isSelected);
	virtual void HandlePress(const foun::RectEdges& pressBounds);
	virtual void HandleRelease(const foun::RectEdges& pressBounds);

	virtual void DrawFill(fill::DrawContext* pDrawContext);
	virtual void WriteFont(font::SymbolWriter* pSymbolWriter);

protected:
	foun::ListNode<MenuItem*> m_listNode;
	MenuPage* m_pParent;
	MenuRequest* m_pRequest;

	const char* m_pLabel;
	foun::Vector2 m_position;

	bool m_isSelected;

	friend class Menu;
	friend class MenuPage;
};

//namespace test
}
//namespace lct
}

#endif//TEST_MENU_ITEM_H
