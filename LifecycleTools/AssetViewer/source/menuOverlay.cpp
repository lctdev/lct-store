#include "menuOverlay.h"
#include "assetViewerMessages.h"

/*
* Internal Constants
*/
static const lct::foun::FloatColor4 BACKER_COLOR = { 0.0f, 0.0f, 0.0f, 0.5f };

#if defined (LCT_WINDOWS) || defined(LCT_OSX)
static const lct::foun::Vector2 PRESS_SIZE = { 0.0f, 0.0f };
#else
static const lct::foun::Vector2 PRESS_SIZE = { 64.0f, 64.0f };
#endif

/*
* Public Instance
*/
MenuOverlay::MenuOverlay()
	: AssetViewerOverlay()

	, m_menu()
	, m_mainMenuPage()
	, m_programOptionsMenuItem()
	, m_modeOptionsMenuItem()
	, m_closeMenuItem()

	, m_projectionTransform()
{
}

MenuOverlay::~MenuOverlay()
{
}

void MenuOverlay::Init()
{
	AssetViewerOverlay::Init();

	lct::font::SheetAsset* pSheetAsset = m_subShared.pAssetContainer->FindAsset<lct::font::SheetAsset>("example_sheet");

	lct::test::Menu::Shared menuShared;
	menuShared.pAllocator = m_shared.pAllocator;
	menuShared.pScreen = m_shared.pScreen;
	menuShared.pGraphicsDevice = m_shared.pGraphicsDevice;
	menuShared.pInputCursor = m_subShared.pCursor;
	menuShared.pFillDrawContext = m_subShared.pFillDrawContext;
	menuShared.pFontDrawContext = m_subShared.pFontDrawContext;
	menuShared.pSheetAsset = pSheetAsset;
	m_menu.SetShared(menuShared);
	m_menu.Initialize();

	m_menu.SetPressSize(PRESS_SIZE);
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
		m_closeMenuItem.GetCallback().Bind(this, &MenuOverlay::OnCloseAction);
		m_mainMenuPage.AddItem(&m_closeMenuItem);
	}
	m_menu.AddPage(&m_mainMenuPage);
	m_menu.Arrange();
	m_menu.ActivatePage("Main");
}

void MenuOverlay::AcquireGraphics()
{
	m_menu.AcquireGraphics();
}

void MenuOverlay::ReleaseGraphics()
{
	m_menu.ReleaseGraphics();
}

bool MenuOverlay::DoesBlockInput()
{
	return true;
}

void MenuOverlay::ReadInput()
{
	m_menu.HandleInput();
}

void MenuOverlay::Update()
{
	const lct::foun::RectEdges& screenEdges = m_shared.pScreen->GetRectEdges();

	lct::foun::Matrix44OrthographicProjection(m_projectionTransform, screenEdges.left, screenEdges.right, screenEdges.bottom, screenEdges.top, -1.0f, 1.0f);
}

void MenuOverlay::Draw()
{
	m_subShared.pFillDrawContext->ActivateRenderState();
	m_subShared.pFillDrawContext->ActivateShader();
	m_subShared.pFillDrawContext->ActivateQuad();
	m_subShared.pFillDrawContext->ActivateProjectionTransform(m_projectionTransform);

	lct::foun::Matrix32 backerWorldTransform;
	lct::foun::Matrix32Identity(backerWorldTransform);
	m_subShared.pFillDrawContext->ActivateWorldTransform(backerWorldTransform);
	const lct::foun::RectCentered& backerRect = m_shared.pScreen->GetRectCentered();
	m_subShared.pFillDrawContext->DrawRect(backerRect, BACKER_COLOR);

	m_menu.Draw();
}

void MenuOverlay::AddProgramOptionsMenuPage(lct::test::MenuPage* pMenuPage)
{
	m_menu.AddPage(pMenuPage);
}

/*
* Protected Instance
*/
void MenuOverlay::OnCloseAction()
{
	lct::fram::Message message;
	message.SetType(MESSAGE_TYPE_PROGRAM_CLOSE_MENU_OVERLAY);
	m_shared.pProgramMessageQueue->EnqueueMessage(message);
}