#ifndef TEST_MENU_H
#define TEST_MENU_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>
#include <foun/foun_rect.h>
#include <foun/foun_vector.h>
#include <foun/foun_color.h>
#include <foun/foun_list.h>

namespace lct
{
namespace inpu
{
	class Cursor;
}
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

class MenuItem;

class Menu
{
public:
	Menu();
	virtual ~Menu();

	void SetBounds(const foun::RectEdges& bounds);
	void SetPosition(const foun::Vector2& position);
	void SetSpacing(f32 spacing);

	void AddItem(MenuItem* pItem);
	void Arrange();

	void HandlePress(const foun::Vector2& position);
	void HandleRelease(const foun::Vector2& position);

	void DrawFill(fill::DrawContext* pDrawContext);
	void WriteFont(font::SymbolWriter* pSymbolWriter);

protected:
	foun::RectEdges m_bounds;
	foun::Vector2 m_position;
	f32 m_spacing;

	foun::List<MenuItem*> m_itemList;
	MenuItem* m_pSelectedItem;
};

//namespace test
}
//namespace lct
}

#endif//TEST_MENU_H
