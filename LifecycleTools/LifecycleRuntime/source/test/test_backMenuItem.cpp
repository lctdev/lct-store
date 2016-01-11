#include <test/test_backMenuItem.h>

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
BackMenuItem::BackMenuItem()
: MenuItem()
, m_button()
, m_buttonOffsetX(DEFAULT_BUTTON_OFFSET_X)
{
	m_button.SetLabel(BUTTON_LABEL);
	m_button.GetTriggerCallback().Bind(this, &BackMenuItem::OnTrigger);
}

BackMenuItem::~BackMenuItem()
{
}

void BackMenuItem::SetOffsets(f32 buttonOffsetX)
{
	m_buttonOffsetX = buttonOffsetX;
}

void BackMenuItem::Arrange()
{
	foun::RectCentered buttonArea;
	buttonArea.x = m_position.x + m_buttonOffsetX;
	buttonArea.y = m_position.y;
	buttonArea.width = BUTTON_WIDTH;
	buttonArea.height = BUTTON_HEIGHT;
	m_button.SetArea(buttonArea);
}

void BackMenuItem::SetIsSelected(bool isSelected)
{
	MenuItem::SetIsSelected(isSelected);

	m_button.ForceUp();
}

void BackMenuItem::HandlePress(const foun::RectEdges& pressBounds)
{
	m_button.HandlePress(pressBounds);
}

void BackMenuItem::HandleRelease(const foun::RectEdges& pressBounds)
{
	m_button.HandleRelease(pressBounds);
}

void BackMenuItem::DrawFill(fill::DrawContext* pDrawContext)
{
	if (m_isSelected)
	{
		m_button.DrawFill(pDrawContext);
	}
}

void BackMenuItem::WriteFont(font::SymbolWriter* pSymbolWriter)
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
void BackMenuItem::OnTrigger()
{
	m_pRequest->backPage = true;
}

//namespace test
}
//namespace lct
}
