#ifndef TEST_MENU_H
#define TEST_MENU_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>
#include <foun/foun_rect.h>
#include <foun/foun_vector.h>
#include <foun/foun_color.h>
#include <foun/foun_list.h>
#include <foun/foun_stack.h>

#include <font/font_symbolBuffer.h>

#include <test/test_menuShared.h>
#include <test/test_backMenuitem.h>

namespace lct
{
namespace grap
{
class Screen;
class Device;
}
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
class DrawContext;
class SymbolWriter;
}

namespace test
{

class MenuPage;

class Menu
{
public:
	Menu();
	virtual ~Menu();

	struct Shared
	{
		foun::Allocator* pAllocator;
		grap::Screen* pScreen;
		grap::Device* pGraphicsDevice;
		inpu::Cursor* pInputCursor;
		fill::DrawContext* pFillDrawContext;
		font::DrawContext* pFontDrawContext;
		font::SheetAsset* pSheetAsset;
	};
	void SetShared(const Shared& shared);

	void Initialize();

	void AcquireGraphics();
	void ReleaseGraphics();

	void SetPosition(const foun::Vector2& position);
	void SetSpacing(f32 spacing);

	void AddPage(MenuPage* pPage);
	void Arrange();
	void ActivatePage(const char* pPageLabel);

	void HandleInput();

	void Draw();

protected:
	Shared m_shared;

	MenuRequest m_request;
	foun::Vector2 m_position;
	f32 m_spacing;

	MenuPage* FindPage(const char* pPageLabel);
	void AdvancePage(MenuPage* pPage);
	void RetractPage();

	foun::List<MenuPage*> m_pageList;
	MenuPage* m_pActivePage;
	static const u32 MAX_PAGE_HISTORY_COUNT = 8;
	foun::Stack<MenuPage*, MAX_PAGE_HISTORY_COUNT> m_pageHistoryStack;

	BackMenuItem m_backItem;	

	static const u32 SYMBOL_BUFFER_COUNT = 2;
	font::SymbolBuffer m_symbolBufferArray[SYMBOL_BUFFER_COUNT];
	u32 m_currSymbolBufferIndex;
};

//namespace test
}
//namespace lct
}

#endif//TEST_MENU_H
