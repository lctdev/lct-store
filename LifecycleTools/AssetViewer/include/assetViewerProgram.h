#ifndef ASSET_VIEWER_PROGRAM_H
#define ASSET_VIEWER_PROGRAM_H

#include <fram/fram_program.h>
#include <fram/fram_message.h>

#include <file/file_packedAccessor.h>
#include <file/file_systemAccessor.h>

#include <util/util_indexMap.h>

#include <pack/pack_assetContainer.h>

#include <pass/pass_assetHandler.h>

#include <fill/fill_drawContext.h>

#include <font/font_assetHandler.h>
#include <font/font_drawContext.h>

#include <spri/spri_drawContext.h>

#include <imag/imag_assetHandler.h>

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
	void LoadAssets();

	void CheckOverlayInput();
	void OpenOverlay();
	void CloseOverlay();

	// owned
	lct::file::Accessor* m_pAccessor;
	lct::file::Accessor* m_pModeAccessor;

	void* m_pAssetBinary;
	lct::pack::AssetContainer m_assetContainer;

	lct::pass::AssetHandler m_passthroughAssetHandler;

	lct::imag::AssetHandler m_imageAssetHandler;

	lct::fill::DrawContext m_fillDrawContext;

	lct::font::AssetHandler m_fontAssetHandler;
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
