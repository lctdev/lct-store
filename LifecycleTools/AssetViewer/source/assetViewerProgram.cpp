#include "assetViewerProgram.h"

#include "cursorOverlay.h"
#include "menuOverlay.h"

#include "spriteViewerMode.h"
#include "soundViewerMode.h"
#include "assetViewerMessages.h"

#include <foun/foun_debug.h>

#include <pass/pass_assets.h>

/*
 * Internal Constants
 */
static const s16 OVERLAY_TOUCH_ACTIVATE_SIZE = 100;

enum OverlayType
{
	OVERLAY_TYPE_CURSOR = 0,
	OVERLAY_TYPE_MENU,
	OVERLAY_TYPE_COUNT
};

/*
 * Public Instance
 */
AssetViewerProgram::AssetViewerProgram()
: Program()
, m_pAccessor(NULL)
, m_pModeAccessor(NULL)
, m_pAssetBinary(NULL)
, m_assetContainer()
, m_passthroughAssetHandler()
, m_imageAssetHandler()
, m_fillDrawContext()
, m_fontAssetHandler()
, m_fontDrawContext()
, m_spriteDrawContext()
#if defined(LCT_WINDOWS) || defined(LCT_OSX)
, m_pMouseReader(NULL)
#elif defined(LCT_IOS) || defined (LCT_ANDROID)
, m_pTouchReader(NULL)
#endif
, m_pCursor(NULL)

, m_pModeNameArray(NULL)
, m_menuPage()
, m_modeMenuItem()
, m_changeModeMenuItem()
{
}

AssetViewerProgram::~AssetViewerProgram()
{
}

/*
 * Protected Instance
 */
void AssetViewerProgram::InitConsole()
{
}

void AssetViewerProgram::InitFiles()
{
#if defined(LCT_WINDOWS)
	m_pAccessor = m_allocator.AllocType<lct::file::SystemAccessor>();
	m_pModeAccessor = m_allocator.AllocType<lct::file::SystemAccessor>();

	char currentDirectoryPath[1024];
	m_pModeAccessor->GetCurrentDirectoryPath(currentDirectoryPath, sizeof(currentDirectoryPath));
	LCT_TRACE("Current directory is: %s\n", currentDirectoryPath);
#elif defined(LCT_ANDROID)
	lct::file::PackedAccessor* pAccessor = m_allocator.AllocType<lct::file::PackedAccessor>();
	lct::file::PackedAccessor* pModeAccessor = m_allocator.AllocType<lct::file::PackedAccessor>();

	pAccessor->SetAssetManager(m_pAndroidApp->activity->assetManager);
	pModeAccessor->SetAssetManager(m_pAndroidApp->activity->assetManager);

	m_pAccessor = pAccessor;
	m_pModeAccessor = pModeAccessor;
#elif defined (LCT_OSX)
    m_pAccessor = m_allocator.AllocType<lct::file::SystemAccessor>();
    m_pModeAccessor = m_allocator.AllocType<lct::file::SystemAccessor>();
    
    char currentDirectoryPath[1024];
    m_pModeAccessor->GetCurrentDirectoryPath(currentDirectoryPath, sizeof(currentDirectoryPath));
    LCT_TRACE("Current directory is: %s\n", currentDirectoryPath);
#elif defined(LCT_IOS)
	lct::file::PackedAccessor* pAccessor = m_allocator.AllocType<lct::file::PackedAccessor>();
	lct::file::PackedAccessor* pModeAccessor = m_allocator.AllocType<lct::file::PackedAccessor>();
	
	m_pAccessor = pAccessor;
	m_pModeAccessor = pModeAccessor;
#endif

	m_pAccessor->SetAllocator(&m_allocator);
	m_pModeAccessor->SetAllocator(&m_modeAllocator);
}

void AssetViewerProgram::InitWindow()
{
	Program::InitWindow();
}

void AssetViewerProgram::InitAssets()
{
	// pack
	m_assetContainer.SetAllocator(&m_allocator);

	// pass
	m_passthroughAssetHandler.SetAllocator(&m_allocator);
	m_passthroughAssetHandler.SetAssetContainer(&m_assetContainer);

	// image
	m_imageAssetHandler.SetAllocator(&m_allocator);
	m_imageAssetHandler.SetGraphicsDevice(&m_graphicsDevice);
	m_imageAssetHandler.SetAssetContainer(&m_assetContainer);

	// fill
	m_fillDrawContext.SetAllocator(&m_allocator);
	m_fillDrawContext.SetGraphicsDevice(&m_graphicsDevice);
	m_fillDrawContext.CreateResources();

	// font
	m_fontAssetHandler.SetAllocator(&m_allocator);
	m_fontAssetHandler.SetAssetContainer(&m_assetContainer);

	m_fontDrawContext.SetAllocator(&m_allocator);
	m_fontDrawContext.SetGraphicsDevice(&m_graphicsDevice);
	m_fontDrawContext.CreateResources();

	// sprite
	m_spriteDrawContext.SetAllocator(&m_allocator);
	m_spriteDrawContext.SetGraphicsDevice(&m_graphicsDevice);
	m_spriteDrawContext.CreateResources();

	LoadAssets();

	Program::InitAssets();
}

void AssetViewerProgram::InitInput()
{
#if defined(LCT_WINDOWS) || defined(LCT_OSX)
	m_pMouseReader = m_allocator.AllocType<lct::inpu::MouseReader>();

	lct::inpu::MouseCursor* pCursor = m_allocator.AllocType<lct::inpu::MouseCursor>();
	pCursor->SetReader(m_pMouseReader);

	m_pCursor = pCursor;
#elif defined(LCT_ANDROID) || defined(LCT_IOS)
	m_pTouchReader = m_allocator.AllocType<lct::inpu::TouchReader>();

	lct::inpu::TouchCursor* pCursor = m_allocator.AllocType<lct::inpu::TouchCursor>();
	pCursor->SetReader(m_pTouchReader);

	m_pCursor = pCursor;
#endif
}

void AssetViewerProgram::InitMiscellaneous()
{
	Program::InitMiscellaneous();

	RegisterMode(lct::fram::ModeFactoryItem::CreateMode<SpriteViewerMode>, "SpriteViewerMode");
	RegisterMode(lct::fram::ModeFactoryItem::CreateMode<SoundViewerMode>, "SoundViewerMode");

	m_pNextModeName = "SpriteViewerMode";


	m_pOverlayArray = m_allocator.AllocTypeArray<lct::fram::Overlay*>(OVERLAY_TYPE_COUNT);

	lct::fram::Overlay::Shared overlayShared;
	overlayShared.pAllocator = &m_modeAllocator;
	overlayShared.pProgramMessageQueue = &m_messageQueue;
	overlayShared.pScreen = &m_screen;
	overlayShared.pGraphicsDevice = &m_graphicsDevice;
	overlayShared.pAudioDevice = &m_audioDevice;
	AssetViewerOverlay::SubShared overlaySubShared;
	overlaySubShared.pCursor = m_pCursor;
	overlaySubShared.pAssetContainer = &m_assetContainer;
	overlaySubShared.pFillDrawContext = &m_fillDrawContext;
	overlaySubShared.pFontDrawContext = &m_fontDrawContext;

	{
		CursorOverlay* pOverlay = m_allocator.AllocType<CursorOverlay>();
		pOverlay->SetShared(overlayShared);
		pOverlay->SetSubShared(overlaySubShared);
		pOverlay->Init();

		m_pOverlayArray[OVERLAY_TYPE_CURSOR] = pOverlay;
	}
	{
		MenuOverlay* pOverlay = m_allocator.AllocType<MenuOverlay>();
		pOverlay->SetShared(overlayShared);
		pOverlay->SetSubShared(overlaySubShared);
		pOverlay->Init();
		pOverlay->AddProgramOptionsMenuPage(&m_menuPage);

		m_pOverlayArray[OVERLAY_TYPE_MENU] = pOverlay;
	}
	m_overlayCount = OVERLAY_TYPE_COUNT;

	if (m_graphicsAcquired)
	{
		for (u32 overlayIndex = 0; overlayIndex < m_overlayCount; ++overlayIndex)
		{
			lct::fram::Overlay* pOverlay = m_pOverlayArray[overlayIndex];
			pOverlay->AcquireGraphics();
		}
	}


	BuildMenu();	
}

void AssetViewerProgram::AcquireGraphics()
{
	Program::AcquireGraphics();

	m_imageAssetHandler.AcquireAllAssetResources();

	m_fillDrawContext.AcquireResources();

	m_fontDrawContext.AcquireResources();

	m_spriteDrawContext.AcquireResources();
}

void AssetViewerProgram::ReleaseGraphics()
{
	m_imageAssetHandler.ReleaseAllAssetResources();

	m_fillDrawContext.ReleaseResources();

	m_fontDrawContext.ReleaseResources();

	m_spriteDrawContext.ReleaseResources();

	Program::ReleaseGraphics();
}

void AssetViewerProgram::ReadSystemMessages()
{
#if defined(LCT_WINDOWS) || defined(LCT_OSX)
	m_pMouseReader->PrepareValues();
#elif defined(LCT_ANDROID) || defined(LCT_IOS)
	m_pTouchReader->PrepareValues();
#endif

	Program::ReadSystemMessages();
}

void AssetViewerProgram::CheckModeChange()
{
	if (m_pNextModeName != NULL)
	{
		if (m_pCurrMode != NULL)
		{
			EndMode();
		}

		BeginMode();
	}
}

void AssetViewerProgram::ReadInput()
{
	Program::ReadInput();

	CheckOverlayInput();
}

void AssetViewerProgram::ConfigureMode()
{
	AssetViewerMode* pAssetViewerMode = static_cast<AssetViewerMode*>(m_pCurrMode);

	AssetViewerMode::SubShared modeSubShared;
	modeSubShared.pAccessor = m_pModeAccessor;
	modeSubShared.pCursor = m_pCursor;
	modeSubShared.pAssetContainer = &m_assetContainer;
	modeSubShared.pFillDrawContext = &m_fillDrawContext;
	modeSubShared.pFontDrawContext = &m_fontDrawContext;
	modeSubShared.pSpriteDrawContext = &m_spriteDrawContext;
	pAssetViewerMode->SetSubShared(modeSubShared);

	Program::ConfigureMode();
}

bool AssetViewerProgram::HandlePlatformMessage(const lct::foun::PlatformMessage& platformMessage)
{
#if defined(LCT_WINDOWS) || defined(LCT_OSX)
	if (m_pMouseReader->HandlePlatformMessage(platformMessage))
	{
		return true;
	}
#elif defined (LCT_IOS) || defined(LCT_ANDROID)
	if (m_pTouchReader->HandlePlatformMessage(platformMessage))
	{
		return true;
	}
#endif
	else
	{
		return Program::HandlePlatformMessage(platformMessage);
	}
}

bool AssetViewerProgram::HandleMessage(const lct::fram::Message& message)
{
	switch (message.GetType())
	{
	case MESSAGE_TYPE_PROGRAM_CLOSE_MENU_OVERLAY:
	{
		DeactivateOverlay(OVERLAY_TYPE_MENU);
		return true;
	}
	case MESSAGE_TYPE_PROGRAM_CHANGE_MODE:
	{
		const ProgramChangeModeContent* pContent = message.GetContent<ProgramChangeModeContent>();
		m_pNextModeName = pContent->pNextModeName;
		EndMode();
		BeginMode();
		return true;
	}
	default:
	{
		return false;
	}
	}
}

/*
 * Private Instance
 */
 void AssetViewerProgram::LoadAssets()
 {
	 static const char* FILE_PATH = "data/programAssets.bin";
	 ssiz fileSize;
	 m_pAssetBinary = m_pAccessor->LoadFile(FILE_PATH, &fileSize);

	 lct::util::BinaryReader binaryReader;
	 binaryReader.SetMemory(m_pAssetBinary, fileSize);
	 lct::pack::PackageWalker packageWalker;
	 packageWalker.AddAssetHandler(&m_passthroughAssetHandler);
	 packageWalker.AddAssetHandler(&m_fontAssetHandler);
	 packageWalker.AddAssetHandler(&m_imageAssetHandler);

	 packageWalker.LoadAllAssets(binaryReader);

	 {
		 static const char* VERTEX_SHADER_NAME = "fill_vertexShader";
		 lct::pass::StringAsset* pVertexShaderStringAsset = m_assetContainer.FindAsset<lct::pass::StringAsset>(VERTEX_SHADER_NAME);
		 static const char* FRAGMENT_SHADER_NAME = "fill_fragmentShader";
		 lct::pass::StringAsset* pFragmentShaderStringAsset = m_assetContainer.FindAsset<lct::pass::StringAsset>(FRAGMENT_SHADER_NAME);

		 m_fillDrawContext.SetShaderBinaries(pVertexShaderStringAsset->pString, pFragmentShaderStringAsset->pString);
	 }

	 {
		 static const char* VERTEX_SHADER_NAME = "font_vertexShader";
		 lct::pass::StringAsset* pVertexShaderStringAsset = m_assetContainer.FindAsset<lct::pass::StringAsset>(VERTEX_SHADER_NAME);
		 static const char* FRAGMENT_SHADER_NAME = "font_fragmentShader";
		 lct::pass::StringAsset* pFragmentShaderStringAsset = m_assetContainer.FindAsset<lct::pass::StringAsset>(FRAGMENT_SHADER_NAME);

		 m_fontDrawContext.SetShaderBinaries(pVertexShaderStringAsset->pString, pFragmentShaderStringAsset->pString);
	 }

	 {
		 static const char* VERTEX_SHADER_NAME = "spri_vertexShader";
		 lct::pass::StringAsset* pVertexShaderStringAsset = m_assetContainer.FindAsset<lct::pass::StringAsset>(VERTEX_SHADER_NAME);
		 static const char* FRAGMENT_SHADER_NAME = "spri_fragmentShader";
		 lct::pass::StringAsset* pFragmentShaderStringAsset = m_assetContainer.FindAsset<lct::pass::StringAsset>(FRAGMENT_SHADER_NAME);

		 m_spriteDrawContext.SetShaderBinaries(pVertexShaderStringAsset->pString, pFragmentShaderStringAsset->pString);
	 }

	 m_fontAssetHandler.FixupAllAssets();
 }

 void AssetViewerProgram::BuildMenu()
 {
	 u32 modeCount = m_modeFactory.GetModeNameCount();
	 m_pModeNameArray = m_allocator.AllocTypeArray<const char*>(modeCount);
	 m_modeFactory.GetAllModeNames(m_pModeNameArray, modeCount);

	 m_menuPage.SetLabel("Program Options");
	 {
		 m_modeMenuItem.SetLabel("Mode");
		 m_modeMenuItem.SetStringArray(m_pModeNameArray);
		 m_modeMenuItem.SetCount(modeCount);
		 m_modeMenuItem.SetIndex(0);
		 m_menuPage.AddItem(&m_modeMenuItem);
	 }
	 {
		 m_changeModeMenuItem.SetLabel("Change Mode");
		 m_changeModeMenuItem.GetCallback().Bind(this, &AssetViewerProgram::OnChangeModeTrigger);
		 m_menuPage.AddItem(&m_changeModeMenuItem);
	 }
	 {
		 m_cursorOverlayMenuItem.SetLabel("Cursor Overlay");
		 m_cursorOverlayMenuItem.SetValue(false);
		 m_cursorOverlayMenuItem.GetCycleCallback().Bind(this, &AssetViewerProgram::OnCursorOverlayChange);
		 m_menuPage.AddItem(&m_cursorOverlayMenuItem);
	 }
 }

 void AssetViewerProgram::CheckOverlayInput()
 {
	 if (!IsOverlayActive(OVERLAY_TYPE_MENU))
	 {
		 s16 x = m_pCursor->GetX();
		 s16 y = m_pCursor->GetY();
		 if (m_pCursor->IsRelease() && (x < OVERLAY_TOUCH_ACTIVATE_SIZE) && (y < OVERLAY_TOUCH_ACTIVATE_SIZE))
		 {
			 ActivateOverlay(OVERLAY_TYPE_MENU);
		 }
	 }
 }

 void AssetViewerProgram::OnChangeModeTrigger()
 {
	 m_pNextModeName = m_pModeNameArray[m_modeMenuItem.GetIndex()];
 }

 void AssetViewerProgram::OnCursorOverlayChange()
 {
	 if (m_cursorOverlayMenuItem.GetValue())
	 {
		 ActivateOverlay(OVERLAY_TYPE_CURSOR);
	 }
	 else
	 {
		 DeactivateOverlay(OVERLAY_TYPE_CURSOR);
	 }
 }
