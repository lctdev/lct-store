#ifndef ASSET_VIEWER_PROGRAM_H
#define ASSET_VIEWER_PROGRAM_H

#include <fram/fram_program.h>

#include <file/file_systemAccessor.h>

namespace lct
{
namespace shad
{
	class ResourceHandler;
}
namespace imag
{
	class AssetProcessor;
	class AssetContainer;
	class ResourceHandler;
}
namespace fill
{
	class DrawContext;
}
namespace font
{
	class AssetProcessor;
	class AssetContainer;
	class DrawContext;
	class ResourceHandler;
}
namespace spri
{
	class DrawContext;
}
namespace inpu
{
	class MouseReader;
	class TouchReader;
	class Cursor;
}
namespace fram
{
	class Message;
}
}

class SpriteViewerOverlay;

class AssetViewerProgram : public lct::fram::Program
{
public:
	AssetViewerProgram();
	virtual ~AssetViewerProgram();

protected:
	virtual void InitConsole();
	virtual void InitFiles();
	virtual void InitWindow();
	virtual void InitAssets();
	virtual void InitInput();
	virtual void InitModes();
	virtual void InitOverlays();
	virtual void InitMessages();

	virtual void AcquireGraphics();
	virtual void ReleaseGraphics();

	virtual void ReadSystemMessages();
	virtual void ReadInput();

	virtual void ConfigureMode();
	virtual bool HandlePlatformMessage(const lct::foun::PlatformMessage& platformMessage);
	virtual bool HandleMessage(const lct::fram::Message& message);

private:
	void LoadFillAssets();
	void LoadFontAssets();
	void LoadSpriteAssets();

	void CheckOverlayInput();
	void OpenOverlay();
	void CloseOverlay();

	lct::file::Accessor* m_pAccessor;
	lct::file::Accessor* m_pModeAccessor;

	lct::shad::ResourceHandler* m_pShaderResourceHandler;

	lct::imag::AssetContainer* m_pImageAssetContainer;
	lct::imag::AssetProcessor* m_pImageAssetProcessor;
	lct::imag::ResourceHandler* m_pImageResourceHandler;
	lct::imag::ResourceHandler* m_pModeImageResourceHandler;

	lct::fill::DrawContext* m_pFillDrawContext;

	lct::font::AssetContainer* m_pFontAssetContainer;
	lct::font::AssetProcessor* m_pFontAssetProcessor;
	lct::font::ResourceHandler* m_pFontResourceHandler;
	lct::font::ResourceHandler* m_pModeFontResourceHandler;

	void* m_pFontAssetBinary;
	lct::font::DrawContext* m_pFontDrawContext;

	lct::spri::DrawContext* m_pSpriteDrawContext;

#if defined(WIN32) || defined(__APPLE__)
	lct::inpu::MouseReader* m_pMouseReader;
#elif defined(__ANDROID__)
	lct::inpu::TouchReader* m_pTouchReader;
#endif
	lct::inpu::Cursor* m_pCursor;

	SpriteViewerOverlay* m_pOverlay;
};

#endif//ASSET_VIEWER_PROGRAM_H
