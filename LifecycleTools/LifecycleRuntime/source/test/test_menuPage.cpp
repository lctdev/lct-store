#include <test/test_menuPage.h>
#include <test/test_menuItem.h>

#include <foun/foun_debug.h>
#include <foun/foun_color.h>
#include <foun/foun_intersection.h>
#include <foun/foun_debug.h>

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
MenuPage::MenuPage()
: m_listNode(this)
, m_pParent(NULL)
, m_pRequest(NULL)
, m_pLabel(NULL)
, m_position()
, m_spacing(0.0f)
, m_itemList()
, m_pSelectedItem(NULL)
{
}

MenuPage::~MenuPage()
{
}

void MenuPage::SetLabel(const char* pLabel)
{
	m_pLabel = pLabel;
}

void MenuPage::SetPosition(const foun::Vector2& position)
{
	m_position = position;
}

void MenuPage::SetSpacing(f32 spacing)
{
	m_spacing = spacing;
}

void MenuPage::AddItem(MenuItem* pItem)
{
	LCT_ASSERT(pItem->m_pParent == NULL);

	pItem->m_pRequest = m_pRequest;

	m_itemList.AddNode(&pItem->m_listNode);	
	pItem->m_pParent = this;
}

void MenuPage::RemoveItem(MenuItem* pItem)
{
	LCT_ASSERT(pItem->m_pParent == this);

	pItem->m_pRequest = NULL;

	m_itemList.RemoveNode(&pItem->m_listNode);
	pItem->m_pParent = NULL;
}

MenuItem* MenuPage::GetLastItem()
{
	foun::ListIterator<MenuItem*> iter = m_itemList.Tail();
	if (!iter.IsEnd())
	{
		return iter.GetValue();
	}
	else
	{
		return NULL;
	}
}

void MenuPage::Arrange()
{
	foun::Vector2 position = m_position;
	for (foun::ListIterator<MenuItem*> iter = m_itemList.Head(); !iter.IsEnd(); iter.Next())
	{
		MenuItem* pItem = iter.GetValue();
		position.y -= m_spacing / 2.0f;
		pItem->SetPosition(position);
		position.y -= m_spacing / 2.0f;
		pItem->Arrange();
	}
}

void MenuPage::HandlePress(const foun::RectEdges& pressBounds)
{
	foun::RectCentered pressRect;
	foun::RectCenteredFromRectEdges(&pressRect, pressBounds);
	foun::Vector2 pressPosition = { pressRect.x, pressRect.y };
	foun::RectEdges itemBounds;
	itemBounds.left = m_position.x;
	itemBounds.right = itemBounds.left + ITEM_INPUT_WIDTH;
	itemBounds.top = m_position.y;
	itemBounds.bottom = itemBounds.top - m_spacing;
	foun::Vector2 itemPosition = { m_position.x, m_position.y - (m_spacing / 2.0f) };
	MenuItem* pOldSelectedItem = m_pSelectedItem;
	MenuItem* pNewSelectedItem = NULL;
	f32 newItemDistSqr = 0.0f;
	for (foun::ListIterator<MenuItem*> iter = m_itemList.Head(); !iter.IsEnd(); iter.Next())
	{
		if (foun::TestIntersection(pressBounds, itemBounds))
		{
			f32 itemDistSqr = foun::Vector2DistanceSquared(pressPosition, itemPosition);
			if ((pNewSelectedItem == NULL) || (itemDistSqr < newItemDistSqr))
			{
				pNewSelectedItem = iter.GetValue();
				newItemDistSqr = itemDistSqr;
			}
		}
		
		itemBounds.top -= m_spacing;
		itemBounds.bottom -= m_spacing;
		itemPosition.y -= m_spacing;
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
		m_pSelectedItem->HandlePress(pressBounds);
	}
}

void MenuPage::HandleRelease(const foun::RectEdges& pressBounds)
{
	if (m_pSelectedItem != NULL)
	{
		m_pSelectedItem->HandleRelease(pressBounds);
	}
}

void MenuPage::DrawFill(fill::DrawContext* pDrawContext)
{
	for (lct::foun::ListIterator<MenuItem*> iter = m_itemList.Head(); !iter.IsEnd(); iter.Next())
	{
		MenuItem* pItem = iter.GetValue();
		pItem->DrawFill(pDrawContext);
	}
}

void MenuPage::WriteFont(font::SymbolWriter* pSymbolWriter)
{
	for (lct::foun::ListIterator<MenuItem*> iter = m_itemList.Head(); !iter.IsEnd(); iter.Next())
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
