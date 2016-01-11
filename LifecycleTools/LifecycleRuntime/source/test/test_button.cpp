#include <test/test_button.h>

#include <foun/foun_debug.h>
#include <foun/foun_intersection.h>

#include <fill/fill_drawContext.h>

#include <font/font_symbolWriter.h>

namespace lct
{
namespace test
{

/*
 * Internal Constants
 */
static const f32 BORDER_SIZE = 4.0f;

static const foun::FloatColor4 BORDER_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };
static const foun::FloatColor4 UP_COLOR = { 0.75f, 0.75f, 0.75f, 1.0f };
static const foun::FloatColor4 DOWN_COLOR = { 0.25, 0.25f, 0.25f, 1.0f };

/*
 * Public Instance
 */
Button::Button()
: m_pLabel(NULL)
, m_triggerCallback()
, m_bounds()
, m_area()
, m_borderArea()
, m_down(false)
{
}

Button::~Button()
{
}

void Button::SetLabel(const char* pLabel)
{
	m_pLabel = pLabel;
}

void Button::SetArea(const foun::RectCentered& area)
{
	m_area = area;
	m_borderArea = area;
	m_borderArea.width += BORDER_SIZE;
	m_borderArea.height += BORDER_SIZE;

	foun::RectEdgesFromRectCentered(&m_bounds, m_area);
}

void Button::HandlePress(const foun::RectEdges& pressBounds)
{
	if (foun::TestIntersection(pressBounds, m_bounds))
	{
		m_down = true;
	}
}

void Button::HandleRelease(const foun::RectEdges& pressBounds)
{
	if (foun::TestIntersection(pressBounds, m_bounds))
	{
		if (m_down)
		{
			if (m_triggerCallback.IsBound())
			{
				m_triggerCallback.Invoke();
			}
		}
	}
	m_down = false;
}

void Button::ForceUp()
{
	m_down = false;
}

void Button::DrawFill(fill::DrawContext* pDrawContext)
{
	pDrawContext->DrawRect(m_borderArea, BORDER_COLOR);
	if (m_down)
	{
		pDrawContext->DrawRect(m_area, DOWN_COLOR);
	}
	else
	{
		pDrawContext->DrawRect(m_area, UP_COLOR);
	}
}

void Button::WriteFont(font::SymbolWriter* pSymbolWriter)
{
	pSymbolWriter->SetCursorX(m_area.x);
	pSymbolWriter->SetCursorY(m_area.y);
	pSymbolWriter->SetHorizontalAlign(font::HORIZONTAL_ALIGN_MIDDLE);
	pSymbolWriter->SetVerticalAlign(font::VERTICAL_ALIGN_MIDDLE);
	pSymbolWriter->SetColor(BORDER_COLOR);
	pSymbolWriter->WriteString(m_pLabel);
}

/*
 * Protected Instance
 */


//namespace test
}
//namespace lct
}
