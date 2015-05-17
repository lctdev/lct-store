#ifndef ASSET_VIEWER_PROGRAM_H
#define ASSET_VIEWER_PROGRAM_H

#include <fram/fram_program.h>
#include <fram/fram_message.h>

#include <file/file_packedAccessor.h>
#include <file/file_systemAccessor.h>

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

	// owned
	lct::file::Accessor* m_pAccessor;
	lct::file::Accessor* m_pModeAccessor;

	lct::shad::ResourceHandler m_shaderResourceHandler;

	lct::imag::AssetContainer m_imageAssetContainer;
	lct::imag::AssetProcessor m_imageAssetProcessor;
	lct::imag::ResourceHandler m_imageResourceHandler;
	lct::imag::ResourceHandler m_modeImageResourceHandler;

	lct::fill::DrawContext m_fillDrawContext;

	lct::font::AssetContainer m_fontAssetContainer;
	lct::font::AssetProcessor m_fontAssetProcessor;
	lct::font::ResourceHandler m_fontResourceHandler;
	lct::font::ResourceHandler m_modeFontResourceHandler;

	void* m_pFontAssetBinary;
	lct::font::DrawContext m_fontDrawContext;

	lct::spri::DrawContext m_spriteDrawContext;

#if defined(LCT_WINDOWS) || defined(LCT_OSX)
	lct::inpu::MouseReader* m_pMouseReader;
#elif defined(LCT_ANDROID)
	lct::inpu::TouchReader* m_touchReader;
#endif
	lct::inpu::Cursor* m_pCursor;

	SpriteViewerOverlay* m_pOverlay;
};

#endif//ASSET_VIEWER_PROGRAM_H
