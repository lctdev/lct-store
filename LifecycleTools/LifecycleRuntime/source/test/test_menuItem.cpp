#include <test/test_menuItem.h>

#include <foun/foun_debug.h>

#include <font/font_symbolWriter.h>

namespace lct
{
namespace test
{

/*
 * Internal Constants
 */
static const foun::FloatColor4 NORMAL_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };
static const foun::FloatColor4 SELECTED_COLOR = { 1.0f, 1.0f, 0.0f, 1.0f };

/*
 * Public Instance
 */
MenuItem::MenuItem()
: m_listNode(this)
, m_pParent(NULL)
, m_pRequest(NULL)
, m_pLabel(NULL)
, m_position()
, m_isSelected(false)
{
}

MenuItem::~MenuItem()
{
}

void MenuItem::SetLabel(const char* pLabel)
{
	m_pLabel = pLabel;
}

void MenuItem::SetPosition(const foun::Vector2& position)
{
	m_position = position;
}

void MenuItem::Arrange()
{
}

void MenuItem::SetIsSelected(bool isSelected)
{
	m_isSelected = isSelected;
}

void MenuItem::HandlePress(const foun::Vector2& position)
{
}

void MenuItem::HandleRelease(const foun::Vector2& position)
{
}

void MenuItem::DrawFill(fill::DrawContext* pDrawContext)
{
}

void MenuItem::WriteFont(font::SymbolWriter* pSymbolWriter)
{
	pSymbolWriter->SetCursor(m_position);
	pSymbolWriter->SetHorizontalAlign(font::HORIZONTAL_ALIGN_LEFT);
	pSymbolWriter->SetVerticalAlign(font::VERTICAL_ALIGN_MIDDLE);
	if (m_isSelected)
	{
		pSymbolWriter->SetColor(SELECTED_COLOR);
	}
	else
	{
		pSymbolWriter->SetColor(NORMAL_COLOR);
	}
	pSymbolWriter->WriteString(m_pLabel);
}

/*
 * Protected Instance
 */


//namespace test
}
//namespace lct
}
