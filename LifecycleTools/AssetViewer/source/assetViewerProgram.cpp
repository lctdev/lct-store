#include "assetViewerProgram.h"

#include "assetViewerOverlay.h"
#include "spriteViewerMode.h"
#include "assetViewerMessages.h"

#include <foun/foun_debug.h>

#include <pass/pass_assets.h>

/*
 * Internal Constants
 */
static const s16 OVERLAY_TOUCH_ACTIVATE_SIZE = 100;

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
#if defined (LCT_WINDOWS) || defined(LCT_OSX)
, m_pMouseReader(NULL)
#elif defined (LCT_ANDROID)
, m_pTouchReader(NULL)
#endif
, m_pCursor(NULL)
, m_pOverlay(NULL)
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
#elif defined(LCT_ANDROID)
	m_pTouchReader = m_allocator.AllocType<lct::inpu::TouchReader>();

	lct::inpu::TouchCursor* pCursor = m_allocator.AllocType<lct::inpu::TouchCursor>();
	pCursor->SetReader(m_pTouchReader);

	m_pCursor = pCursor;
#endif
}

void AssetViewerProgram::InitModes()
{
	RegisterMode(lct::fram::ModeFactoryItem::CreateMode<SpriteViewerMode>, "SpriteViewerMode");

	m_pNextModeName = "SpriteViewerMode";

	Program::InitModes();
}

void AssetViewerProgram::InitOverlays()
{
	m_pOverlay = m_allocator.AllocType<SpriteViewerOverlay>();
	m_pOverlay->SetAllocator(&m_allocator);
	m_pOverlay->SetGraphicsDevice(&m_graphicsDevice);
	m_pOverlay->SetScreen(&m_screen);
	m_pOverlay->SetProgramMessageQueue(&m_messageQueue);
	m_pOverlay->SetInputCursor(m_pCursor);
	m_pOverlay->SetSharedAssetContainer(&m_assetContainer);
	m_pOverlay->SetFillDrawContext(&m_fillDrawContext);	
	m_pOverlay->SetFontDrawContext(&m_fontDrawContext);
	m_pOverlay->Init();

	if (m_graphicsAcquired)
	{
		m_pOverlay->AcquireGraphics();
	}

	Program::InitOverlays();
}

void AssetViewerProgram::InitMessages()
{
	Program::InitMessages();
}

void AssetViewerProgram::AcquireGraphics()
{
	Program::AcquireGraphics();

	m_imageAssetHandler.AcquireAllAssetResources();

	m_fillDrawContext.AcquireResources();

	m_fontDrawContext.AcquireResources();

	m_spriteDrawContext.AcquireResources();

	if (m_pOverlay != NULL) // should go in base...?
	{
		m_pOverlay->AcquireGraphics();
	}

	LCT_TRACE("AssetViewerProgram::AcquireGraphics Texture Resource Count: %u\n", m_graphicsDevice.GetUsedTextureResourceCount());
	LCT_TRACE("AssetViewerProgram::AcquireGraphics Vertex Resource Count: %u\n", m_graphicsDevice.GetUsedVertexResourceCount());
	LCT_TRACE("AssetViewerProgram::AcquireGraphics Index Resource Count: %u\n", m_graphicsDevice.GetUsedIndexResourceCount());
}

void AssetViewerProgram::ReleaseGraphics()
{
	m_imageAssetHandler.ReleaseAllAssetResources();

	m_fillDrawContext.ReleaseResources();

	m_fontDrawContext.ReleaseResources();

	m_spriteDrawContext.ReleaseResources();

	if (m_pOverlay != NULL) // should go in base...?
	{
		m_pOverlay->ReleaseGraphics();
	}

	LCT_TRACE("AssetViewerProgram::ReleaseGraphics Texture Resource Count: %u\n", m_graphicsDevice.GetUsedTextureResourceCount());
	LCT_TRACE("AssetViewerProgram::ReleaseGraphics Vertex Resource Count: %u\n", m_graphicsDevice.GetUsedVertexResourceCount());
	LCT_TRACE("AssetViewerProgram::ReleaseGraphics Index Resource Count: %u\n", m_graphicsDevice.GetUsedIndexResourceCount());

	Program::ReleaseGraphics();
}

void AssetViewerProgram::ReadSystemMessages()
{
#if defined(LCT_WINDOWS) || defined(LCT_OSX)
	m_pMouseReader->PrepareValues();
#elif defined(LCT_ANDROID)
	m_pTouchReader->PrepareValues();
#endif

	Program::ReadSystemMessages();
}

void AssetViewerProgram::ReadInput()
{
	Program::ReadInput();

	CheckOverlayInput();
}

void AssetViewerProgram::ConfigureMode()
{
	AssetViewerMode* pAssetViewerMode = static_cast<AssetViewerMode*>(m_pCurrMode);

	pAssetViewerMode->SetAccessor(m_pModeAccessor);	
	pAssetViewerMode->SetCursor(m_pCursor);
	pAssetViewerMode->SetSharedAssetContainer(&m_assetContainer);
	pAssetViewerMode->SetFillDrawContext(&m_fillDrawContext);
	pAssetViewerMode->SetFontDrawContext(&m_fontDrawContext);
	pAssetViewerMode->SetSpriteDrawContext(&m_spriteDrawContext);

	Program::ConfigureMode();
}

bool AssetViewerProgram::HandlePlatformMessage(const lct::foun::PlatformMessage& platformMessage)
{
#if defined(LCT_WINDOWS) || defined(LCT_OSX)
	if (m_pMouseReader->HandlePlatformMessage(platformMessage))
	{
		return true;
	}
#elif defined(LCT_ANDROID)
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
	case MESSAGE_TYPE_PROGRAM_CLOSE_OVERLAY:
	{
		CloseOverlay();
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
	 u32 fileSize;
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

 void AssetViewerProgram::CheckOverlayInput()
 {
	 if (m_pCurrOverlay == NULL)
	 {
		 s16 x = m_pCursor->GetX();
		 s16 y = m_pCursor->GetY();
		 if (m_pCursor->IsRelease() && (x < OVERLAY_TOUCH_ACTIVATE_SIZE) && (y < OVERLAY_TOUCH_ACTIVATE_SIZE))
		 {
			 m_pCurrOverlay = m_pOverlay;
		 }
	 }
 }

 void AssetViewerProgram::OpenOverlay()
 {

 }

 void AssetViewerProgram::CloseOverlay()
 {
	 m_pCurrOverlay = NULL;
 }
