#include "assetViewerOverlay.h"
#include "assetViewerMessages.h"

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
, m_pInputCursor(NULL)
, m_pFillDrawContext(NULL)
, m_pFontAssetContainer(NULL)
, m_pFontDrawContext(NULL)

, m_symbolBufferArray()
, m_currSymbolBufferIndex(0)
, m_symbolWriter()
, m_backActionCallback()
, m_menu()
, m_projectionTransform()
{
}

SpriteViewerOverlay::~SpriteViewerOverlay()
{
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

void SpriteViewerOverlay::SetFontDrawContext(lct::font::DrawContext* pDrawContext)
{
	m_pFontDrawContext = pDrawContext;
}

void SpriteViewerOverlay::Init()
{
	Overlay::Init();

	lct::font::SheetAsset* pSheetAsset = m_pFontAssetContainer->FindSheetAsset("example_sheet");

	for (u32 bufferIndex = 0; bufferIndex < SYMBOL_BUFFER_COUNT; ++bufferIndex)
	{
		lct::font::SymbolBuffer& symbolBuffer = m_symbolBufferArray[bufferIndex];
		symbolBuffer.SetSheetAsset(pSheetAsset);
		symbolBuffer.CreateStructure(SYMBOL_BUFFER_QUAD_COUNT, m_pAllocator);
	}
	m_currSymbolBufferIndex = 0;

	m_backActionCallback.Bind(this, &SpriteViewerOverlay::OnBackAction);

	m_menu.SetSpacing(pSheetAsset->pSheetData->lineHeight * 2.0f);
	{
		lct::test::ActionMenuItem* pItem = m_pAllocator->AllocType<lct::test::ActionMenuItem>();
		pItem->SetLabel("Program Options");
		m_menu.AddItem(pItem);
	}
	{
		lct::test::ActionMenuItem* pItem = m_pAllocator->AllocType<lct::test::ActionMenuItem>();
		pItem->SetLabel("Mode Options");
		m_menu.AddItem(pItem);
	}
	{
		lct::test::ActionMenuItem* pItem = m_pAllocator->AllocType<lct::test::ActionMenuItem>();
		pItem->SetLabel("Back");
		pItem->SetCallback(&m_backActionCallback);
		m_menu.AddItem(pItem);
	}
	m_menu.Arrange();
}

void SpriteViewerOverlay::AcquireGraphics()
{
	for (u32 bufferIndex = 0; bufferIndex < SYMBOL_BUFFER_COUNT; ++bufferIndex)
	{
		m_symbolBufferArray[bufferIndex].AcquireResources(m_pGraphicsDevice);
	}
}

void SpriteViewerOverlay::ReleaseGraphics()
{
	for (u32 bufferIndex = 0; bufferIndex < SYMBOL_BUFFER_COUNT; ++bufferIndex)
	{
		m_symbolBufferArray[bufferIndex].ReleaseResources(m_pGraphicsDevice);
	}
}

void SpriteViewerOverlay::ReadInput()
{
	lct::foun::Vector2 localCursorPosition;
	localCursorPosition.x = static_cast<f32>(m_pInputCursor->GetX());
	localCursorPosition.y = -static_cast<f32>(m_pInputCursor->GetY());
	if (m_pInputCursor->IsPress())
	{
		m_menu.HandlePress(localCursorPosition);
	}
	if (m_pInputCursor->IsRelease())
	{
		m_menu.HandleRelease(localCursorPosition);
	}
}

void SpriteViewerOverlay::Update()
{
	lct::font::SymbolBuffer& symbolBuffer = m_symbolBufferArray[m_currSymbolBufferIndex];
	symbolBuffer.ResetQuads();
	m_symbolWriter.SetBuffer(&symbolBuffer);

	m_menu.WriteFont(&m_symbolWriter);
	symbolBuffer.RefreshResources(m_pGraphicsDevice);

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
	m_menu.DrawFill(m_pFillDrawContext);

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

	lct::font::SymbolBuffer& symbolBuffer = m_symbolBufferArray[m_currSymbolBufferIndex];
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
