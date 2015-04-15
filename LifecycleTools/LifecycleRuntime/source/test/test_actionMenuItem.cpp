#include <test/test_actionMenuItem.h>

#include <foun/foun_debug.h>

#include <font/font_symbolWriter.h>

namespace lct
{
namespace test
{

/*
 * Internal Constants
 */
static const char* BUTTON_LABEL = "OK";
static const f32 DEFAULT_BUTTON_OFFSET_X = 300.0f;
static const f32 BUTTON_WIDTH = 40.0f;
static const f32 BUTTON_HEIGHT = 32.0f;

/*
 * Public Instance
 */
ActionMenuItem::ActionMenuItem()
: MenuItem()
, m_button()
, m_buttonOffsetX(DEFAULT_BUTTON_OFFSET_X)
{
	m_button.SetLabel(BUTTON_LABEL);
}

ActionMenuItem::~ActionMenuItem()
{
}

void ActionMenuItem::SetOffsets(f32 buttonOffsetX)
{
	m_buttonOffsetX = buttonOffsetX;
}

void ActionMenuItem::SetCallback(foun::Callback<int, int>* pTriggerCallback)
{
	m_button.SetTriggerCallback(pTriggerCallback);
}

void ActionMenuItem::Arrange()
{
	foun::RectCentered buttonArea;
	buttonArea.x = m_position.x + m_buttonOffsetX;
	buttonArea.y = m_position.y;
	buttonArea.width = BUTTON_WIDTH;
	buttonArea.height = BUTTON_HEIGHT;
	m_button.SetArea(buttonArea);
}

void ActionMenuItem::SetIsSelected(bool isSelected)
{
	MenuItem::SetIsSelected(isSelected);

	m_button.ForceUp();
}

void ActionMenuItem::HandlePress(const foun::Vector2& position)
{
	m_button.HandlePress(position);
}

void ActionMenuItem::HandleRelease(const foun::Vector2& position)
{
	m_button.HandleRelease(position);
}

void ActionMenuItem::DrawFill(fill::DrawContext* pDrawContext)
{
	if (m_isSelected)
	{
		m_button.DrawFill(pDrawContext);
	}
}

void ActionMenuItem::WriteFont(font::SymbolWriter* pSymbolWriter)
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


//namespace test
}
//namespace lct
}
