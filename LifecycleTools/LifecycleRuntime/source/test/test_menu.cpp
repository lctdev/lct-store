#include <test/test_menu.h>
#include <test/test_menuItem.h>

#include <foun/foun_debug.h>
#include <foun/foun_color.h>
#include <foun/foun_intersection.h>

#include <inpu/inpu_cursor.h>

#include <font/font_symbolWriter.h>

namespace lct
{
namespace test
{

/*
 * Internal Constants
 */
static const f32 ITEM_INPUT_WIDTH = 128.0f;

/*
 * Public Instance
 */
Menu::Menu()
: m_bounds()
, m_position()
, m_spacing(0.0f)
, m_itemList()
, m_pSelectedItem(NULL)
{
}

Menu::~Menu()
{
}

void Menu::SetBounds(const foun::RectEdges& bounds)
{
	m_bounds = bounds;
}

void Menu::SetPosition(const foun::Vector2& position)
{
	m_position = position;
}

void Menu::SetSpacing(f32 spacing)
{
	m_spacing = spacing;
}

void Menu::AddItem(MenuItem* pItem)
{
	m_itemList.AddNode(&pItem->m_listNode);
}

void Menu::Arrange()
{
	foun::Vector2 position = m_position;
	for (foun::ListIterator<MenuItem*> iter = m_itemList.Begin(); !iter.IsEnd(); iter.Next())
	{
		MenuItem* pItem = iter.GetValue();
		position.y -= m_spacing / 2.0f;
		pItem->SetPosition(position);
		position.y -= m_spacing / 2.0f;
		iter.GetValue()->Arrange();
	}
}

void Menu::HandlePress(const foun::Vector2& position)
{
	foun::RectEdges itemBounds;
	itemBounds.left = m_position.x;
	itemBounds.right = itemBounds.left + ITEM_INPUT_WIDTH;
	itemBounds.top = m_position.y;
	itemBounds.bottom = itemBounds.top - m_spacing;
	MenuItem* pOldSelectedItem = m_pSelectedItem;
	MenuItem* pNewSelectedItem = NULL;
	for (foun::ListIterator<MenuItem*> iter = m_itemList.Begin(); !iter.IsEnd(); iter.Next())
	{
		if (foun::TestIntersection(position, itemBounds))
		{
			pNewSelectedItem = iter.GetValue();
		}
		itemBounds.top -= m_spacing;
		itemBounds.bottom -= m_spacing;
	}

	if (pNewSelectedItem != NULL)
	{
		if (pOldSelectedItem != pNewSelectedItem)
		{
			if (pOldSelectedItem != NULL)
			{
				pOldSelectedItem->SetIsSelected(false);
			}
			pNewSelectedItem->SetIsSelected(true);
			m_pSelectedItem = pNewSelectedItem;
		}
	}

	if (m_pSelectedItem != NULL)
	{
		m_pSelectedItem->HandlePress(position);
	}
}

void Menu::HandleRelease(const foun::Vector2& position)
{
	if (m_pSelectedItem != NULL)
	{
		m_pSelectedItem->HandleRelease(position);
	}
}

void Menu::DrawFill(fill::DrawContext* pDrawContext)
{
	for (lct::foun::ListIterator<MenuItem*> iter = m_itemList.Begin(); !iter.IsEnd(); iter.Next())
	{
		MenuItem* pItem = iter.GetValue();
		pItem->DrawFill(pDrawContext);
	}
}

void Menu::WriteFont(font::SymbolWriter* pSymbolWriter)
{
	for (lct::foun::ListIterator<MenuItem*> iter = m_itemList.Begin(); !iter.IsEnd(); iter.Next())
	{
		MenuItem* pItem = iter.GetValue();
		pItem->WriteFont(pSymbolWriter);
	}
}

/*
 * Protected Instance
 */


//namespace test
}
//namespace lct
}
