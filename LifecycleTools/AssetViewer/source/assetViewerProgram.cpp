#include "assetViewerProgram.h"

#include "assetViewerOverlay.h"
#include "spriteViewerMode.h"
#include "assetViewerMessages.h"

#include <fram/fram_message.h>

#include <file/file_packedAccessor.h>

#include <shad/shad_resourceHandler.h>

#include <util/util_indexMap.h>

#include <fill/fill_drawContext.h>

#include <font/font_assetProcessor.h>
#include <font/font_assetContainer.h>
#include <font/font_drawContext.h>
#include <font/font_resourceHandler.h>

#include <spri/spri_drawContext.h>

#include <imag/imag_assetProcessor.h>
#include <imag/imag_assetContainer.h>
#include <imag/imag_resourceHandler.h>

#include <pack/pack_packageWalker.h>

#include <util/util_binaryReader.h>

#include <inpu/inpu_mouseReader.h>
#include <inpu/inpu_mouseCursor.h>
#include <inpu/inpu_touchReader.h>
#include <inpu/inpu_touchCursor.h>

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
, m_pShaderResourceHandler(NULL)
, m_pImageAssetContainer(NULL)
, m_pImageAssetProcessor(NULL)
, m_pImageResourceHandler(NULL)
, m_pModeImageResourceHandler(NULL)
, m_pFillDrawContext(NULL)
, m_pFontAssetContainer(NULL)
, m_pFontAssetProcessor(NULL)
, m_pFontResourceHandler(NULL)
, m_pModeFontResourceHandler(NULL)
, m_pFontAssetBinary(NULL)
, m_pFontDrawContext(NULL)
, m_pSpriteDrawContext(NULL)
#if defined (WIN32) || defined(__APPLE__)
, m_pMouseReader(NULL)
#elif defined (__ANDROID__)
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
#if defined(WIN32)
	m_pAccessor = m_allocator.AllocType<lct::file::SystemAccessor>();
	m_pModeAccessor = m_allocator.AllocType<lct::file::SystemAccessor>();

#if defined(LCT_DEBUG)
	int x = 0;
	x++;
	printf("Hello");
#endif

	char currentDirectoryPath[1024];
	m_pModeAccessor->GetCurrentDirectoryPath(currentDirectoryPath, sizeof(currentDirectoryPath));
	LCT_TRACE("Current directory is: %s\n", currentDirectoryPath);
#elif defined(__ANDROID__)
	lct::file::PackedAccessor* pAccessor = m_allocator.AllocType<lct::file::PackedAccessor>();
	lct::file::PackedAccessor* pModeAccessor = m_allocator.AllocType<lct::file::PackedAccessor>();

	pAccessor->SetAssetManager(m_pAndroidApp->activity->assetManager);
	pModeAccessor->SetAssetManager(m_pAndroidApp->activity->assetManager);

	m_pAccessor = pAccessor;
	m_pModeAccessor = pModeAccessor;
#elif defined (__APPLE__)
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
	m_pShaderResourceHandler = m_allocator.AllocType<lct::shad::ResourceHandler>();
	m_pShaderResourceHandler->SetAllocator(&m_allocator);

	// image
	m_pImageAssetContainer = m_allocator.AllocType<lct::imag::AssetContainer>();
	m_pImageAssetContainer->SetAllocator(&m_allocator);

	m_pImageResourceHandler = m_allocator.AllocType<lct::imag::ResourceHandler>();
	m_pModeImageResourceHandler = m_allocator.AllocType<lct::imag::ResourceHandler>();

	m_pImageAssetProcessor = m_allocator.AllocType<lct::imag::AssetProcessor>();
	m_pImageAssetProcessor->SetAllocator(&m_allocator);
	m_pImageAssetProcessor->SetResourceHandler(m_pImageResourceHandler);
	m_pImageAssetProcessor->SetAssetContainer(m_pImageAssetContainer);

	// fill
	m_pFillDrawContext = m_allocator.AllocType<lct::fill::DrawContext>();
	m_pFillDrawContext->SetAllocator(&m_allocator);
	m_pFillDrawContext->SetShaderResourceHandler(m_pShaderResourceHandler);
	m_pFillDrawContext->CreateResources();

	LoadFillAssets();

	// font
	m_pFontAssetContainer = m_allocator.AllocType<lct::font::AssetContainer>();
	m_pFontAssetContainer->SetAllocator(&m_allocator);

	m_pFontAssetProcessor = m_allocator.AllocType<lct::font::AssetProcessor>();
	m_pFontAssetProcessor->SetAllocator(&m_allocator);
	m_pFontAssetProcessor->SetAssetContainer(m_pFontAssetContainer);

	m_pFontResourceHandler = m_allocator.AllocType<lct::font::ResourceHandler>();
	m_pModeFontResourceHandler = m_allocator.AllocType<lct::font::ResourceHandler>();

	m_pFontDrawContext = m_allocator.AllocType<lct::font::DrawContext>();
	m_pFontDrawContext->SetAllocator(&m_allocator);
	m_pFontDrawContext->SetShaderResourceHandler(m_pShaderResourceHandler);
	m_pFontDrawContext->CreateResources();

	LoadFontAssets();

	// sprite
	m_pSpriteDrawContext = m_allocator.AllocType<lct::spri::DrawContext>();
	m_pSpriteDrawContext->SetAllocator(&m_allocator);
	m_pSpriteDrawContext->SetShaderResourceHandler(m_pShaderResourceHandler);
	m_pSpriteDrawContext->CreateResources();

	LoadSpriteAssets();

	Program::InitAssets();
}

void AssetViewerProgram::InitInput()
{
#if defined(WIN32) || defined(__APPLE__)
	m_pMouseReader = m_allocator.AllocType<lct::inpu::MouseReader>();

	lct::inpu::MouseCursor* pCursor = m_allocator.AllocType<lct::inpu::MouseCursor>();
	pCursor->SetReader(m_pMouseReader);

	m_pCursor = pCursor;
#elif defined(__ANDROID__)
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
	m_pOverlay->SetFillDrawContext(m_pFillDrawContext);
	m_pOverlay->SetFontAssetContainer(m_pFontAssetContainer);
	m_pOverlay->SetFontResourceHandler(m_pFontResourceHandler);
	m_pOverlay->SetFontDrawContext(m_pFontDrawContext);
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

	m_pImageAssetProcessor->AcquireAllAssetResources();

	m_pFillDrawContext->AcquireResources();

	m_pFontDrawContext->AcquireResources();

	m_pSpriteDrawContext->AcquireResources();

	if (m_pOverlay != NULL) // should go in base...?
	{
		m_pOverlay->AcquireGraphics();
	}

	LCT_TRACE("AssetViewerProgram::AcquireGraphics Image Texture Resource Count: %u\n", m_pImageResourceHandler->GetTextureResourceCount());
	LCT_TRACE("AssetViewerProgram::AcquireGraphics Font Quad Resource Count: %u\n", m_pFontResourceHandler->GetQuadResourceCount());
}

void AssetViewerProgram::ReleaseGraphics()
{
	m_pImageAssetProcessor->ReleaseAllAssetResources();

	m_pFillDrawContext->ReleaseResources();

	m_pFontDrawContext->ReleaseResources();

	m_pSpriteDrawContext->ReleaseResources();

	if (m_pOverlay != NULL) // should go in base...?
	{
		m_pOverlay->ReleaseGraphics();
	}

	LCT_TRACE("AssetViewerProgram::ReleaseGraphics Image Texture Resource Count: %u\n", m_pImageResourceHandler->GetTextureResourceCount());
	LCT_TRACE("AssetViewerProgram::ReleaseGraphics Font Quad Resource Count: %u\n", m_pFontResourceHandler->GetQuadResourceCount());

	Program::ReleaseGraphics();
}

void AssetViewerProgram::ReadSystemMessages()
{
#if defined(WIN32) || defined(__APPLE__)
	m_pMouseReader->PrepareValues();
#elif defined(__ANDROID__)
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
	pAssetViewerMode->SetSpriteDrawContext(m_pSpriteDrawContext);
	pAssetViewerMode->SetCursor(m_pCursor);
	pAssetViewerMode->SetImageResourceHandler(m_pModeImageResourceHandler);
	pAssetViewerMode->SetFillDrawContext(m_pFillDrawContext);
	pAssetViewerMode->SetFontAssetContainer(m_pFontAssetContainer);
	pAssetViewerMode->SetFontResourceHandler(m_pModeFontResourceHandler);
	pAssetViewerMode->SetFontDrawContext(m_pFontDrawContext);

	Program::ConfigureMode();
}

bool AssetViewerProgram::HandlePlatformMessage(const lct::foun::PlatformMessage& platformMessage)
{
#if defined(WIN32) || defined(__APPLE__)
	if (m_pMouseReader->HandlePlatformMessage(platformMessage))
	{
		return true;
	}
#elif defined(__ANDROID__)
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

	 m_pFillDrawContext->SetShaderBinaries(pVertexShaderBinary, pFragmentShaderBinary);
}

 void AssetViewerProgram::LoadFontAssets()
 {
	 static const char* FILE_PATH = "data/font/package.bin";
	 u32 fileSize;
	 m_pFontAssetBinary = m_pAccessor->LoadFile(FILE_PATH, &fileSize);
	 lct::util::BinaryReader binaryReader;
	 binaryReader.SetMemory(m_pFontAssetBinary, fileSize);
	 lct::pack::PackageWalker packageWalker;
	 packageWalker.AddAssetHandler(m_pFontAssetProcessor);
	 packageWalker.AddAssetHandler(m_pImageAssetProcessor);

	 packageWalker.LoadAllAssets(binaryReader);

	 m_pFontAssetProcessor->FixupAllAssets(*m_pImageAssetContainer);

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

	 m_pFontDrawContext->SetShaderBinaries(pVertexShaderBinary, pFragmentShaderBinary);
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

	 m_pSpriteDrawContext->SetShaderBinaries(pVertexShaderBinary, pFragmentShaderBinary);
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
