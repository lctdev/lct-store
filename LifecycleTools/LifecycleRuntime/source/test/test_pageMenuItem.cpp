#include <test/test_pageMenuItem.h>

#include <foun/foun_debug.h>

#include <font/font_symbolWriter.h>

namespace lct
{
namespace test
{

/*
* Internal Constants
*/
static const char* BUTTON_LABEL = "GO";
static const f32 DEFAULT_BUTTON_OFFSET_X = 300.0f;
static const f32 BUTTON_WIDTH = 40.0f;
static const f32 BUTTON_HEIGHT = 32.0f;

/*
* Public Instance
*/
PageMenuItem::PageMenuItem()
: MenuItem()
, m_button()
, m_buttonOffsetX(DEFAULT_BUTTON_OFFSET_X)
{
	m_button.SetLabel(BUTTON_LABEL);
	m_button.GetTriggerCallback().Bind(this, &PageMenuItem::OnTrigger);
}

PageMenuItem::~PageMenuItem()
{
}

void PageMenuItem::SetOffsets(f32 buttonOffsetX)
{
	m_buttonOffsetX = buttonOffsetX;
}

void PageMenuItem::Arrange()
{
	foun::RectCentered buttonArea;
	buttonArea.x = m_position.x + m_buttonOffsetX;
	buttonArea.y = m_position.y;
	buttonArea.width = BUTTON_WIDTH;
	buttonArea.height = BUTTON_HEIGHT;
	m_button.SetArea(buttonArea);
}

void PageMenuItem::SetIsSelected(bool isSelected)
{
	MenuItem::SetIsSelected(isSelected);

	m_button.ForceUp();
}

void PageMenuItem::HandlePress(const foun::RectEdges& pressBounds)
{
	m_button.HandlePress(pressBounds);
}

void PageMenuItem::HandleRelease(const foun::RectEdges& pressBounds)
{
	m_button.HandleRelease(pressBounds);
}

void PageMenuItem::DrawFill(fill::DrawContext* pDrawContext)
{
	if (m_isSelected)
	{
		m_button.DrawFill(pDrawContext);
	}
}

void PageMenuItem::WriteFont(font::SymbolWriter* pSymbolWriter)
{
	MenuItem::WriteFont(pSymbolWriter);

	if (m_isSelected)
	{
		m_button.WriteFont(pSymbolWriter);
	}
}

/*
* Protected Instance
*/
void PageMenuItem::OnTrigger()
{
	m_pRequest->pChangePageLabel = m_pLabel;
}

//namespace test
}
//namespace lct
}
