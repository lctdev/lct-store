#include <test/test_cycleMenuItem.h>

#include <foun/foun_debug.h>

#include <font/font_symbolWriter.h>

namespace lct
{
namespace test
{

/*
 * Internal Constants
 */
static const f32 DEFAULT_VALUE_OFFSET_X = 300.0f;
static const foun::FloatColor VALUE_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };

static const char* LEFT_BUTTON_LABEL = "<";
static const char* RIGHT_BUTTON_LABEL = ">";
static const f32 DEFAULT_LEFT_BUTTON_OFFSET_X = 200.0f;
static const f32 DEFAULT_RIGHT_BUTTON_OFFSET_X = 400.0f;
static const f32 BUTTON_WIDTH = 40.0f;
static const f32 BUTTON_HEIGHT = 32.0f;

/*
 * Public Instance
 */
CycleMenuItem::CycleMenuItem()
: MenuItem()
, m_leftButton()
, m_rightButton()
, m_leftTriggerCallback()
, m_rightTriggerCallback()
, m_valueOffsetX(DEFAULT_VALUE_OFFSET_X)
, m_leftButtonOffsetX(DEFAULT_LEFT_BUTTON_OFFSET_X)
, m_rightButtonoffsetX(DEFAULT_RIGHT_BUTTON_OFFSET_X)
{
	m_leftButton.SetLabel(LEFT_BUTTON_LABEL);
	m_rightButton.SetLabel(RIGHT_BUTTON_LABEL);

	m_leftTriggerCallback.Bind(this, &CycleMenuItem::OnLeftTrigger);
	m_rightTriggerCallback.Bind(this, &CycleMenuItem::OnRightTrigger);

	m_leftButton.SetTriggerCallback(&m_leftTriggerCallback);
	m_rightButton.SetTriggerCallback(&m_rightTriggerCallback);
}

CycleMenuItem::~CycleMenuItem()
{
}

void CycleMenuItem::SetOffsets(f32 valueOffsetX, f32 leftButtonOffsetX, f32 rightButtonOffsetX)
{
	m_valueOffsetX = valueOffsetX;
	m_leftButtonOffsetX = leftButtonOffsetX;
	m_rightButtonoffsetX = rightButtonOffsetX;
}

void CycleMenuItem::Arrange()
{
	{
		foun::RectCentered buttonArea;
		buttonArea.x = m_position.x + m_leftButtonOffsetX;
		buttonArea.y = m_position.y;
		buttonArea.width = BUTTON_WIDTH;
		buttonArea.height = BUTTON_HEIGHT;
		m_leftButton.SetArea(buttonArea);
	}
	{
		foun::RectCentered buttonArea;
		buttonArea.x = m_position.x + m_rightButtonoffsetX;
		buttonArea.y = m_position.y;
		buttonArea.width = BUTTON_WIDTH;
		buttonArea.height = BUTTON_HEIGHT;
		m_rightButton.SetArea(buttonArea);
	}
}

void CycleMenuItem::SetIsSelected(bool isSelected)
{
	MenuItem::SetIsSelected(isSelected);

	m_leftButton.ForceUp();
	m_rightButton.ForceUp();
}

void CycleMenuItem::HandlePress(const foun::Vector2& position)
{
	m_leftButton.HandlePress(position);
	m_rightButton.HandlePress(position);
}

void CycleMenuItem::HandleRelease(const foun::Vector2& position)
{
	m_leftButton.HandleRelease(position);
	m_rightButton.HandleRelease(position);
}

void CycleMenuItem::DrawFill(fill::DrawContext* pDrawContext)
{
	if (m_isSelected)
	{
		m_leftButton.DrawFill(pDrawContext);
		m_rightButton.DrawFill(pDrawContext);
	}
}

void CycleMenuItem::WriteFont(font::SymbolWriter* pSymbolWriter)
{
	MenuItem::WriteFont(pSymbolWriter);

	const char* pValueString = GetValueString();

	pSymbolWriter->SetCursorX(m_position.x + m_valueOffsetX);
	pSymbolWriter->SetCursorY(m_position.y);
	pSymbolWriter->SetHorizontalAlign(font::HORIZONTAL_ALIGN_MIDDLE);
	pSymbolWriter->SetVerticalAlign(font::VERTICAL_ALIGN_MIDDLE);
	pSymbolWriter->SetColor(VALUE_COLOR);
	pSymbolWriter->WriteString(pValueString);

	if (m_isSelected)
	{
		m_leftButton.WriteFont(pSymbolWriter);
		m_rightButton.WriteFont(pSymbolWriter);
	}
}

/*
 * Protected Instance
 */
int CycleMenuItem::OnLeftTrigger(int parameter)
{
	CycleLeft();

	return 0;
}

int CycleMenuItem::OnRightTrigger(int parameter)
{
	CycleRight();

	return 0;
}

//namespace test
}
//namespace lct
}
