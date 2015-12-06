#include "assetViewerOverlay.h"
#include "assetViewerMessages.h"

/*
 * Internal Constants
 */
static const lct::foun::FloatColor BACKER_COLOR = { 0.0f, 0.0f, 0.0f, 0.5f };

/*
 * Public Instance
 */
AssetViewerOverlay::AssetViewerOverlay()
: Overlay()
, m_pInputCursor(NULL)
, m_pSharedAssetContainer(NULL)
, m_pFillDrawContext(NULL)
, m_pFontDrawContext(NULL)

, m_menu()
, m_mainMenuPage()
, m_programOptionsMenuItem()
, m_modeOptionsMenuItem()
, m_closeMenuItem()

, m_projectionTransform()
{
}

AssetViewerOverlay::~AssetViewerOverlay()
{
}

void AssetViewerOverlay::SetInputCursor(lct::inpu::Cursor* pCursor)
{
	m_pInputCursor = pCursor;
}

void AssetViewerOverlay::SetSharedAssetContainer(lct::pack::AssetContainer* pAssetContainer)
{
	m_pSharedAssetContainer = pAssetContainer;
}

void AssetViewerOverlay::SetFillDrawContext(lct::fill::DrawContext* pDrawContext)
{
	m_pFillDrawContext = pDrawContext;
}

void AssetViewerOverlay::SetFontDrawContext(lct::font::DrawContext* pDrawContext)
{
	m_pFontDrawContext = pDrawContext;
}

void AssetViewerOverlay::Init()
{
	Overlay::Init();

	lct::font::SheetAsset* pSheetAsset = m_pSharedAssetContainer->FindAsset<lct::font::SheetAsset>("example_sheet");

	lct::test::Menu::Shared menuShared;
	menuShared.pAllocator = m_pAllocator;
	menuShared.pScreen = m_pScreen;
	menuShared.pGraphicsDevice = m_pGraphicsDevice;
	menuShared.pInputCursor = m_pInputCursor;
	menuShared.pFillDrawContext = m_pFillDrawContext;
	menuShared.pFontDrawContext = m_pFontDrawContext;
	menuShared.pSheetAsset = pSheetAsset;
	m_menu.SetShared(menuShared);
	m_menu.Initialize();

	m_menu.SetSpacing(pSheetAsset->pSheetData->lineHeight * 2.0f);

	m_mainMenuPage.SetLabel("Main");
	{
		m_programOptionsMenuItem.SetLabel("Program Options");
		m_mainMenuPage.AddItem(&m_programOptionsMenuItem);
	}
	{
		m_modeOptionsMenuItem.SetLabel("Mode Options");
		m_mainMenuPage.AddItem(&m_modeOptionsMenuItem);
	}
	{
		m_closeMenuItem.SetLabel("Close");
		m_closeMenuItem.GetCallback().Bind(this, &AssetViewerOverlay::OnCloseAction);
		m_mainMenuPage.AddItem(&m_closeMenuItem);
	}
	m_menu.AddPage(&m_mainMenuPage);
	m_menu.Arrange();
	m_menu.ActivatePage("Main");
}

void AssetViewerOverlay::AcquireGraphics()
{
	m_menu.AcquireGraphics();
}

void AssetViewerOverlay::ReleaseGraphics()
{
	m_menu.ReleaseGraphics();
}

void AssetViewerOverlay::ReadInput()
{
	m_menu.HandleInput();
}

void AssetViewerOverlay::Update()
{
	const lct::foun::RectEdges& screenEdges = m_pScreen->GetRectEdges();

	lct::foun::Matrix44OrthographicProjection(m_projectionTransform, screenEdges.left, screenEdges.right, screenEdges.bottom, screenEdges.top, -1.0f, 1.0f);
}

void AssetViewerOverlay::Draw()
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
	
	// TEMP!!!
#if 0
	static const lct::foun::FloatColor PRESS_COLOR = { 0.0, 1.0f, 0.0f, 1.0f };
	static const lct::foun::FloatColor DOWN_COLOR = { 1.0, 1.0f, 0.0f, 1.0f };
	static const lct::foun::FloatColor RELEASE_COLOR = { 1.0, 0.0f, 0.0f, 1.0f };
	bool drawTest = false;
	lct::foun::FloatColor testColor;
	if (m_pInputCursor->IsPress())
	{
		drawTest = true;
		testColor = PRESS_COLOR;
	}
	else if (m_pInputCursor->IsDown())
	{
		drawTest = true;
		testColor = DOWN_COLOR;
	}
	else if (m_pInputCursor->IsRelease())
	{
		drawTest = true;
		testColor = RELEASE_COLOR;
	}
	if (drawTest)
	{
		lct::foun::Matrix32 pressWorldTransform;
		lct::foun::Matrix32Identity(pressWorldTransform);
		m_pFillDrawContext->ActivateWorldTransform(pressWorldTransform);
		
		const lct::foun::RectEdges& screenEdges = m_pScreen->GetRectEdges();
		lct::foun::RectCentered testRect;
		testRect.x = static_cast<f32>(m_pInputCursor->GetX()) + screenEdges.left;
		testRect.y = -static_cast<f32>(m_pInputCursor->GetY()) + screenEdges.top;
		testRect.width = 64.0f;
		testRect.height = 64.0f;

		m_pFillDrawContext->DrawRect(testRect, testColor);
	}
	
	m_pFillDrawContext->DeactivateQuad();
#endif

	m_menu.Draw();
}

void AssetViewerOverlay::AddProgramOptionsMenuPage(lct::test::MenuPage* pMenuPage)
{
	m_menu.AddPage(pMenuPage);
}

/*
 * Protected Instance
 */
void AssetViewerOverlay::OnCloseAction()
{
	lct::fram::Message message;
	message.SetType(MESSAGE_TYPE_PROGRAM_CLOSE_OVERLAY);
	m_pProgramMessageQueue->EnqueueMessage(message);
}