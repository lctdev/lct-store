#include <test/test_menu.h>
#include <test/test_menuPage.h>
#include <test/test_menuItem.h>

#include <foun/foun_debug.h>
#include <foun/foun_color.h>
#include <foun/foun_intersection.h>
#include <foun/foun_string.h>
#include <foun/foun_debug.h>

#include <grap/grap_screen.h>

#include <inpu/inpu_cursor.h>

#include <fill/fill_drawContext.h>

#include <font/font_symbolWriter.h>
#include <font/font_drawContext.h>

namespace lct
{
namespace test
{

/*
 * Internal Constants
 */
static const char* BACK_ITEM_LABEL = "Back";

static const f32 ITEM_INPUT_WIDTH = 128.0f;

static const u32 SYMBOL_BUFFER_QUAD_COUNT = 512;

/*
 * Public Instance
 */
Menu::Menu()
: m_shared()

, m_request()
, m_position()
, m_spacing(0.0f)
, m_pageList()
, m_pActivePage(NULL)
, m_pageHistoryStack()
, m_backItem()

, m_symbolBufferArray()
, m_currSymbolBufferIndex(0)
{
	m_backItem.SetLabel(BACK_ITEM_LABEL);
}

Menu::~Menu()
{
}

void Menu::SetShared(const Shared& shared)
{
	m_shared = shared;
}

void Menu::Initialize()
{
	for (u32 bufferIndex = 0; bufferIndex < SYMBOL_BUFFER_COUNT; ++bufferIndex)
	{
		lct::font::SymbolBuffer& symbolBuffer = m_symbolBufferArray[bufferIndex];
		symbolBuffer.SetSheetAsset(m_shared.pSheetAsset);
		symbolBuffer.CreateStructure(SYMBOL_BUFFER_QUAD_COUNT, m_shared.pAllocator);
	}
	m_currSymbolBufferIndex = 0;
}

void Menu::AcquireGraphics()
{
	for (u32 bufferIndex = 0; bufferIndex < SYMBOL_BUFFER_COUNT; ++bufferIndex)
	{
		m_symbolBufferArray[bufferIndex].AcquireResources(m_shared.pGraphicsDevice);
	}
}

void Menu::ReleaseGraphics()
{
	for (u32 bufferIndex = 0; bufferIndex < SYMBOL_BUFFER_COUNT; ++bufferIndex)
	{
		m_symbolBufferArray[bufferIndex].ReleaseResources(m_shared.pGraphicsDevice);
	}
}

void Menu::SetPosition(const foun::Vector2& position)
{
	m_position = position;

	Arrange();
}

void Menu::SetSpacing(f32 spacing)
{
	m_spacing = spacing;

	Arrange();
}

void Menu::AddPage(MenuPage* pPage)
{
	LCT_ASSERT(pPage->m_pParent == NULL);

	pPage->m_pRequest = &m_request;
	for (foun::ListIterator<MenuItem*> iter = pPage->m_itemList.Head(); !iter.IsEnd(); iter.Next())
	{
		iter.GetValue()->m_pRequest = &m_request;
	}

	pPage->SetPosition(m_position);
	pPage->SetSpacing(m_spacing);
	pPage->Arrange();

	m_pageList.AddNode(&pPage->m_listNode);	
	pPage->m_pParent = this;
}

void Menu::Arrange()
{
	for (foun::ListIterator<MenuPage*> iter = m_pageList.Head(); !iter.IsEnd(); iter.Next())
	{
		MenuPage* pPage = iter.GetValue();
		pPage->SetPosition(m_position);
		pPage->SetSpacing(m_spacing);
		pPage->Arrange();
	}
}

void Menu::ActivatePage(const char* pPageLabel)
{
	MenuPage* pPage = FindPage(pPageLabel);
	if (pPage != NULL)
	{
		AdvancePage(pPage);
	}
}

void Menu::HandleInput()
{
	lct::foun::Vector2 localCursorPosition;
	localCursorPosition.x = static_cast<f32>(m_shared.pInputCursor->GetX());
	localCursorPosition.y = -static_cast<f32>(m_shared.pInputCursor->GetY());
	if (m_shared.pInputCursor->IsPress())
	{
		m_pActivePage->HandlePress(localCursorPosition);
	}
	if (m_shared.pInputCursor->IsRelease())
	{
		m_pActivePage->HandleRelease(localCursorPosition);
	}

	if (m_request.pChangePageLabel != NULL)
	{
		ActivatePage(m_request.pChangePageLabel);
		m_request.pChangePageLabel = NULL;
	}
	else if (m_request.backPage)
	{
		RetractPage();
		m_request.backPage = false;
	}
}

void Menu::Draw()
{
	const lct::foun::RectEdges& screenEdges = m_shared.pScreen->GetRectEdges();
	lct::foun::Matrix44 projectionTransform;
	lct::foun::Matrix44OrthographicProjection(projectionTransform, screenEdges.left, screenEdges.right, screenEdges.bottom, screenEdges.top, -1.0f, 1.0f);
	lct::foun::Matrix32 worldTransform;
	lct::foun::Matrix32Translate(worldTransform, screenEdges.left, screenEdges.top);

	if (m_pActivePage != NULL)
	{
		{
			m_shared.pFillDrawContext->ActivateRenderState();
			m_shared.pFillDrawContext->ActivateShader();
			m_shared.pFillDrawContext->ActivateQuad();
			m_shared.pFillDrawContext->ActivateProjectionTransform(projectionTransform);

			m_shared.pFillDrawContext->ActivateWorldTransform(worldTransform);
			m_pActivePage->DrawFill(m_shared.pFillDrawContext);

			m_shared.pFillDrawContext->DeactivateQuad();
		}

		{
			lct::font::SymbolBuffer& symbolBuffer = m_symbolBufferArray[m_currSymbolBufferIndex];
			symbolBuffer.ResetQuads();
			font::SymbolWriter symbolWriter;
			symbolWriter.SetBuffer(&symbolBuffer);
			m_pActivePage->WriteFont(&symbolWriter);
			symbolBuffer.RefreshResources(m_shared.pGraphicsDevice);

			m_shared.pFontDrawContext->ActivateRenderState();
			m_shared.pFontDrawContext->ActivateShader();
			m_shared.pFontDrawContext->ActivateProjectionTransform(projectionTransform);
			m_shared.pFontDrawContext->ActivateWorldTransform(worldTransform);
			
			if (symbolBuffer.GetQuadCount() > 0)
			{
				m_shared.pFontDrawContext->ActivateSymbolBuffer(symbolBuffer);

				m_shared.pFontDrawContext->DrawSymbolBuffer(symbolBuffer);

				m_shared.pFontDrawContext->DeactivateSymbolBuffer(symbolBuffer);
			}
			m_currSymbolBufferIndex = 1 - m_currSymbolBufferIndex;
		}
	}
}

/*
 * Protected Instance
 */
MenuPage* Menu::FindPage(const char* pPageLabel)
{
	for (foun::ListIterator<MenuPage*> iter = m_pageList.Head(); !iter.IsEnd(); iter.Next())
	{
		MenuPage* pPage = iter.GetValue();
		if (LCT_STRCMP(pPage->GetLabel(), pPageLabel) == 0)
		{
			return pPage;
		}
	}
	return NULL;
}

void Menu::AdvancePage(MenuPage* pPage)
{
	if (m_pActivePage != NULL)
	{
		if (m_pageHistoryStack.GetValueCount() > 0)
		{
			m_pActivePage->RemoveItem(&m_backItem);
		}

		if (m_pageHistoryStack.GetValueCount() < MAX_PAGE_HISTORY_COUNT)
		{
			m_pageHistoryStack.PushValue(m_pActivePage);
		}
	}	

	m_pActivePage = pPage;

	if (m_pageHistoryStack.GetValueCount() > 0)
	{
		m_pActivePage->AddItem(&m_backItem);
		m_pActivePage->Arrange();
	}
}

void Menu::RetractPage()
{
	if (m_pageHistoryStack.GetValueCount() > 0)
	{
		m_pActivePage->RemoveItem(&m_backItem);

		m_pActivePage = m_pageHistoryStack.PopValue();

		if (m_pageHistoryStack.GetValueCount() > 0)
		{
			m_pActivePage->AddItem(&m_backItem);
			m_pActivePage->Arrange();
		}
	}
}

//namespace test
}
//namespace lct
}
