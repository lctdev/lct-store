#ifndef TEST_BUTTON_H
#define TEST_BUTTON_H

#include <foun/foun_primitives.h>
#include <foun/foun_color.h>
#include <foun/foun_list.h>
#include <foun/foun_rect.h>
#include <foun/foun_vector.h>
#include <foun/foun_callback.h>

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

class Button
{
public:
	Button();
	virtual ~Button();

	void SetLabel(const char* pLabel);
	void SetTriggerCallback(foun::Callback<int, int>* pTriggerCallback);
	void SetArea(const foun::RectCentered& area);

	void HandlePress(const foun::Vector2& position);
	void HandleRelease(const foun::Vector2& position);
	void ForceUp();

	void DrawFill(fill::DrawContext* pDrawContext);
	void WriteFont(font::SymbolWriter* pSymbolWriter);

protected:
	const char* m_pLabel;
	foun::Callback<int, int>* m_pTriggerCallback;
	foun::RectEdges m_bounds;
	foun::RectCentered m_area;
	foun::RectCentered m_borderArea;

	bool m_down;
};

//namespace test
}
//namespace lct
}

#endif//TEST_MENU_ITEM_H
