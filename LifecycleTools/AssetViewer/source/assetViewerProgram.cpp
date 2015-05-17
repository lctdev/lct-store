#include "assetViewerProgram.h"

#include "assetViewerOverlay.h"
#include "spriteViewerMode.h"
#include "assetViewerMessages.h"

#include <foun/foun_debug.h>

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
, m_shaderResourceHandler()
, m_imageAssetContainer()
, m_imageAssetProcessor()
, m_imageResourceHandler()
, m_modeImageResourceHandler()
, m_fillDrawContext()
, m_fontAssetContainer()
, m_fontAssetProcessor()
, m_fontResourceHandler()
, m_modeFontResourceHandler()
, m_pFontAssetBinary(NULL)
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
	// shader
	m_shaderResourceHandler.SetAllocator(&m_allocator);

	// image
	m_imageAssetContainer.SetAllocator(&m_allocator);

	m_imageAssetProcessor.SetAllocator(&m_allocator);
	m_imageAssetProcessor.SetResourceHandler(&m_imageResourceHandler);
	m_imageAssetProcessor.SetAssetContainer(&m_imageAssetContainer);

	// fill
	m_fillDrawContext.SetAllocator(&m_allocator);
	m_fillDrawContext.SetShaderResourceHandler(&m_shaderResourceHandler);
	m_fillDrawContext.CreateResources();

	LoadFillAssets();

	// font
	m_fontAssetContainer.SetAllocator(&m_allocator);

	m_fontAssetProcessor.SetAllocator(&m_allocator);
	m_fontAssetProcessor.SetAssetContainer(&m_fontAssetContainer);

	m_fontDrawContext.SetAllocator(&m_allocator);
	m_fontDrawContext.SetShaderResourceHandler(&m_shaderResourceHandler);
	m_fontDrawContext.CreateResources();

	LoadFontAssets();

	// sprite
	m_spriteDrawContext.SetAllocator(&m_allocator);
	m_spriteDrawContext.SetShaderResourceHandler(&m_shaderResourceHandler);
	m_spriteDrawContext.CreateResources();

	LoadSpriteAssets();

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
	m_pOverlay->SetScreen(&m_screen);
	m_pOverlay->SetProgramMessageQueue(&m_messageQueue);
	m_pOverlay->SetInputCursor(m_pCursor);
	m_pOverlay->SetFillDrawContext(&m_fillDrawContext);
	m_pOverlay->SetFontAssetContainer(&m_fontAssetContainer);
	m_pOverlay->SetFontResourceHandler(&m_fontResourceHandler);
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

	m_imageAssetProcessor.AcquireAllAssetResources();

	m_fillDrawContext.AcquireResources();

	m_fontDrawContext.AcquireResources();

	m_spriteDrawContext.AcquireResources();

	if (m_pOverlay != NULL) // should go in base...?
	{
		m_pOverlay->AcquireGraphics();
	}

	LCT_TRACE("AssetViewerProgram::AcquireGraphics Image Texture Resource Count: %u\n", m_imageResourceHandler.GetTextureResourceCount());
	LCT_TRACE("AssetViewerProgram::AcquireGraphics Font Quad Resource Count: %u\n", m_fontResourceHandler.GetQuadResourceCount());
}

void AssetViewerProgram::ReleaseGraphics()
{
	m_imageAssetProcessor.ReleaseAllAssetResources();

	m_fillDrawContext.ReleaseResources();

	m_fontDrawContext.ReleaseResources();

	m_spriteDrawContext.ReleaseResources();

	if (m_pOverlay != NULL) // should go in base...?
	{
		m_pOverlay->ReleaseGraphics();
	}

	LCT_TRACE("AssetViewerProgram::ReleaseGraphics Image Texture Resource Count: %u\n", m_imageResourceHandler.GetTextureResourceCount());
	LCT_TRACE("AssetViewerProgram::ReleaseGraphics Font Quad Resource Count: %u\n", m_fontResourceHandler.GetQuadResourceCount());

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
	pAssetViewerMode->SetSpriteDrawContext(&m_spriteDrawContext);
	pAssetViewerMode->SetCursor(m_pCursor);
	pAssetViewerMode->SetImageResourceHandler(&m_modeImageResourceHandler);
	pAssetViewerMode->SetFillDrawContext(&m_fillDrawContext);
	pAssetViewerMode->SetFontAssetContainer(&m_fontAssetContainer);
	pAssetViewerMode->SetFontResourceHandler(&m_modeFontResourceHandler);
	pAssetViewerMode->SetFontDrawContext(&m_fontDrawContext);

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
void AssetViewerProgram::LoadFillAssets()
{
	 void* pVertexShaderBinary = NULL;
	 {
		 static const char* FILE_PATH = "data/shad/fill_vertexShader.glsl";
		 u32 stringSize;
		 pVertexShaderBinary = m_pAccessor->LoadFileString(FILE_PATH, &stringSize);
	 }

	 void* pFragmentShaderBinary = NULL;
	 {
		 static const char* FILE_PATH = "data/shad/fill_fragmentShader.glsl";
		 u32 stringSize;
		 pFragmentShaderBinary = m_pAccessor->LoadFileString(FILE_PATH, &stringSize);
	 }

	 m_fillDrawContext.SetShaderBinaries(pVertexShaderBinary, pFragmentShaderBinary);
}

 void AssetViewerProgram::LoadFontAssets()
 {
	 static const char* FILE_PATH = "data/font/package.bin";
	 u32 fileSize;
	 m_pFontAssetBinary = m_pAccessor->LoadFile(FILE_PATH, &fileSize);
	 lct::util::BinaryReader binaryReader;
	 binaryReader.SetMemory(m_pFontAssetBinary, fileSize);
	 lct::pack::PackageWalker packageWalker;
	 packageWalker.AddAssetHandler(&m_fontAssetProcessor);
	 packageWalker.AddAssetHandler(&m_imageAssetProcessor);

	 packageWalker.LoadAllAssets(binaryReader);

	 m_fontAssetProcessor.FixupAllAssets(m_imageAssetContainer);

	 void* pVertexShaderBinary = NULL;
	 {
		 static const char* FILE_PATH = "data/shad/font_vertexShader.glsl";
		 u32 stringSize;
		 pVertexShaderBinary = m_pAccessor->LoadFileString(FILE_PATH, &stringSize);
	 }

	 void* pFragmentShaderBinary = NULL;
	 {
		 static const char* FILE_PATH = "data/shad/font_fragmentShader.glsl";
		 u32 stringSize;
		 pFragmentShaderBinary = m_pAccessor->LoadFileString(FILE_PATH, &stringSize);
	 }

	 m_fontDrawContext.SetShaderBinaries(pVertexShaderBinary, pFragmentShaderBinary);
 }

 void AssetViewerProgram::LoadSpriteAssets()
 {
	 void* pVertexShaderBinary = NULL;
	 {
		 static const char* FILE_PATH = "data/shad/spri_vertexShader.glsl";
		 u32 stringSize;
		 pVertexShaderBinary = m_pAccessor->LoadFileString(FILE_PATH, &stringSize);
	 }

	 void* pFragmentShaderBinary = NULL;
	 {
		 static const char* FILE_PATH = "data/shad/spri_fragmentShader.glsl";
		 u32 stringSize;
		 pFragmentShaderBinary = m_pAccessor->LoadFileString(FILE_PATH, &stringSize);
	 }

	 m_spriteDrawContext.SetShaderBinaries(pVertexShaderBinary, pFragmentShaderBinary);
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
