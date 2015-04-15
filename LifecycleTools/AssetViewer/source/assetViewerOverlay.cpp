#include "assetViewerOverlay.h"
#include "assetViewerMessages.h"

#include <foun/foun_callback.h>

#include <inpu/inpu_cursor.h>

#include <fill/fill_drawContext.h>

#include <font/font_assets.h>
#include <font/font_assetContainer.h>
#include <font/font_data.h>
#include <font/font_symbolBuffer.h>
#include <font/font_symbolWriter.h>
#include <font/font_drawContext.h>

#include <test/test_menu.h>
#include <test/test_actionMenuItem.h>

#include <fram/fram_screen.h>
#include <fram/fram_messageQueue.h>
#include <fram/fram_message.h>

/*
 * Internal Constants
 */
static const u32 SYMBOL_BUFFER_QUAD_COUNT = 512;

static const lct::foun::FloatColor BACKER_COLOR = { 0.0f, 0.0f, 0.0f, 0.5f };

/*
 * Public Instance
 */
SpriteViewerOverlay::SpriteViewerOverlay()
: Overlay()
, m_pProgramMessageQueue(NULL)
, m_pInputCursor(NULL)
, m_pFillDrawContext(NULL)
, m_pFontAssetContainer(NULL)
, m_pFontResourceHandler(NULL)
, m_pSymbolBufferArray(NULL)
, m_currSymbolBufferIndex(0)
, m_pSymbolWriter(NULL)
, m_pFontDrawContext(NULL)
, m_backActionCallback()
, m_pMenu(NULL)
, m_projectionTransform()
{
}

SpriteViewerOverlay::~SpriteViewerOverlay()
{
}

void SpriteViewerOverlay::SetProgramMessageQueue(lct::fram::MessageQueue* pMessageQueue)
{
	m_pProgramMessageQueue = pMessageQueue;
}

void SpriteViewerOverlay::SetInputCursor(lct::inpu::Cursor* pCursor)
{
	m_pInputCursor = pCursor;
}

void SpriteViewerOverlay::SetFillDrawContext(lct::fill::DrawContext* pDrawContext)
{
	m_pFillDrawContext = pDrawContext;
}

void SpriteViewerOverlay::SetFontAssetContainer(lct::font::AssetContainer* pAssetContainer)
{
	m_pFontAssetContainer = pAssetContainer;
}

void SpriteViewerOverlay::SetFontResourceHandler(lct::font::ResourceHandler* pResourceHandler)
{
	m_pFontResourceHandler = pResourceHandler;
}

void SpriteViewerOverlay::SetFontDrawContext(lct::font::DrawContext* pDrawContext)
{
	m_pFontDrawContext = pDrawContext;
}

void SpriteViewerOverlay::Init()
{
	Overlay::Init();

	lct::font::SheetAsset* pSheetAsset = m_pFontAssetContainer->FindSheetAsset("example_sheet");

	m_pSymbolBufferArray = m_pAllocator->AllocTypeArray<lct::font::SymbolBuffer>(SYMBOL_BUFFER_COUNT);
	for (u32 bufferIndex = 0; bufferIndex < SYMBOL_BUFFER_COUNT; ++bufferIndex)
	{
		lct::font::SymbolBuffer& symbolBuffer = m_pSymbolBufferArray[bufferIndex];
		symbolBuffer.SetAllocator(m_pAllocator);
		symbolBuffer.SetResourceHandler(m_pFontResourceHandler);
		symbolBuffer.SetSheetAsset(pSheetAsset);
		symbolBuffer.CreateResources(SYMBOL_BUFFER_QUAD_COUNT);
	}
	m_currSymbolBufferIndex = 0;

	m_pSymbolWriter = m_pAllocator->AllocType<lct::font::SymbolWriter>();

	m_backActionCallback.Bind(this, &SpriteViewerOverlay::OnBackAction);

	m_pMenu = m_pAllocator->AllocType<lct::test::Menu>();
	m_pMenu->SetSpacing(pSheetAsset->pSheetData->lineHeight * 2.0f);
	{
		lct::test::ActionMenuItem* pItem = m_pAllocator->AllocType<lct::test::ActionMenuItem>();
		pItem->SetLabel("Program Options");
		m_pMenu->AddItem(pItem);
	}
	{
		lct::test::ActionMenuItem* pItem = m_pAllocator->AllocType<lct::test::ActionMenuItem>();
		pItem->SetLabel("Mode Options");
		m_pMenu->AddItem(pItem);
	}
	{
		lct::test::ActionMenuItem* pItem = m_pAllocator->AllocType<lct::test::ActionMenuItem>();
		pItem->SetLabel("Back");
		pItem->SetCallback(&m_backActionCallback);
		m_pMenu->AddItem(pItem);
	}
	m_pMenu->Arrange();
}

void SpriteViewerOverlay::AcquireGraphics()
{
	for (u32 bufferIndex = 0; bufferIndex < SYMBOL_BUFFER_COUNT; ++bufferIndex)
	{
		m_pSymbolBufferArray[bufferIndex].AcquireResources();
	}
}

void SpriteViewerOverlay::ReleaseGraphics()
{
	for (u32 bufferIndex = 0; bufferIndex < SYMBOL_BUFFER_COUNT; ++bufferIndex)
	{
		m_pSymbolBufferArray[bufferIndex].ReleaseResources();
	}
}

void SpriteViewerOverlay::ReadInput()
{
	lct::foun::Vector2 localCursorPosition;
	localCursorPosition.x = static_cast<f32>(m_pInputCursor->GetX());
	localCursorPosition.y = -static_cast<f32>(m_pInputCursor->GetY());
	if (m_pInputCursor->IsPress())
	{
		m_pMenu->HandlePress(localCursorPosition);
	}
	if (m_pInputCursor->IsRelease())
	{
		m_pMenu->HandleRelease(localCursorPosition);
	}
}

void SpriteViewerOverlay::Update()
{
	lct::font::SymbolBuffer& symbolBuffer = m_pSymbolBufferArray[m_currSymbolBufferIndex];
	symbolBuffer.ResetResources();
	m_pSymbolWriter->SetBuffer(&symbolBuffer);

	m_pMenu->WriteFont(m_pSymbolWriter);
	symbolBuffer.UpdateResources();

	const lct::foun::RectEdges& screenEdges = m_pScreen->GetRectEdges();

	lct::foun::Matrix44OrthographicProjection(m_projectionTransform, screenEdges.left, screenEdges.right, screenEdges.bottom, screenEdges.top, -1.0f, 1.0f);
}

void SpriteViewerOverlay::Draw()
{
	m_pFillDrawContext->ActivateRenderState();
	m_pFillDrawContext->ActivateShader();
	m_pFillDrawContext->ActivateQuad();
	m_pFillDrawContext->ActivateProjectionTransform(m_projectionTransform);

	lct::foun::Matrix32 backerWorldTransform;
	lct::foun::Matrix32Identity(backerWorldTransform);
	m_pFillDrawContext->ActivateWorldTransform(backerWorldTransform);
	const lct::foun::RectCentered& backerRect = m_pScreen->GetRectCentered();
	m_pFillDrawContext->DrawRect(backerRect, BACKER_COLOR);

	const lct::foun::RectEdges& screenEdges = m_pScreen->GetRectEdges();
	lct::foun::Matrix32 menuWorldTransform;
	lct::foun::Matrix32Translate(menuWorldTransform, screenEdges.left, screenEdges.top);
	m_pFillDrawContext->ActivateWorldTransform(menuWorldTransform);
	m_pMenu->DrawFill(m_pFillDrawContext);

	/*lct::foun::RectCentered testRect;
	testRect.x = 0.0f;
	testRect.y = 0.0f;
	testRect.width = 100.0f;
	testRect.height = 100.0f;
	lct::foun::FloatColor testColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_pFillDrawContext->DrawRect(testRect, testColor);*/

	/*// TEMP!!!
	if (m_pInputCursor->IsPress())
	{
		const lct::foun::RectEdges& screenEdges = m_pScreen->GetRectEdges();
		lct::foun::RectCentered testRect;
		testRect.x = static_cast<f32>(m_pInputCursor->GetX()) + screenEdges.left;
		testRect.y = -static_cast<f32>(m_pInputCursor->GetY()) + screenEdges.top;
		testRect.width = 64.0f;
		testRect.height = 64.0f;

		lct::foun::FloatColor testColor = { 1.0f, 1.0f, 1.0f, 1.0f };

		m_pSpriteDrawContext->DrawRect(testRect, testColor);
	}*/

	m_pFontDrawContext->ActivateRenderState();
	m_pFontDrawContext->ActivateShader();
	m_pFontDrawContext->ActivateProjectionTransform(m_projectionTransform);
	m_pFontDrawContext->ActivateWorldTransform(menuWorldTransform);

	lct::font::SymbolBuffer& symbolBuffer = m_pSymbolBufferArray[m_currSymbolBufferIndex];
	if (symbolBuffer.GetQuadCount() > 0)
	{
		m_pFontDrawContext->ActivateSymbolBuffer(symbolBuffer);
		m_pFontDrawContext->DrawSymbolBuffer(symbolBuffer);
	}
	m_currSymbolBufferIndex = 1 - m_currSymbolBufferIndex;
}

int SpriteViewerOverlay::OnBackAction(int parameter)
{
	lct::fram::Message message;
	message.SetType(MESSAGE_TYPE_PROGRAM_CLOSE_OVERLAY);
	m_pProgramMessageQueue->EnqueueMessage(message);
	return 0;
}

/*
 * Protected Instance
 */
