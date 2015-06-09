#ifndef ASSET_VIEWER_MODE_H
#define ASSET_VIEWER_MODE_H

#include <foun/foun_debug.h>

#include <file/file_accessor.h>

#include <inpu/inpu_cursor.h>

#include <imag/imag_resourceHandler.h>

#include <fill/fill_drawContext.h>

#include <font/font_assetContainer.h>
#include <font/font_drawContext.h>

#include <spri/spri_drawContext.h>

#include <fram/fram_mode.h>


class AssetViewerMode : public lct::fram::Mode
{
public:
	AssetViewerMode();
	virtual ~AssetViewerMode();

	void SetAccessor(lct::file::Accessor* pAccessor);
	void SetCursor(lct::inpu::Cursor* pCursor);
	void SetImageResourceHandler(lct::imag::ResourceHandler* pResourceHandler);
	void SetFillDrawContext(lct::fill::DrawContext* pDrawContext);
	void SetFontAssetContainer(lct::font::AssetContainer* pAssetContainer);
	void SetFontDrawContext(lct::font::DrawContext* pDrawContext);
	void SetSpriteDrawContext(lct::spri::DrawContext* pDrawContext);

	virtual void Init();

	virtual void AcquireGraphics();
	virtual void ReleaseGraphics();

protected:
	// shared
	lct::file::Accessor* m_pAccessor;

	lct::inpu::Cursor* m_pCursor;

	lct::imag::ResourceHandler* m_pImageResourceHandler;

	lct::fill::DrawContext* m_pFillDrawContext;

	lct::font::AssetContainer* m_pFontAssetContainer;
	lct::font::DrawContext* m_pFontDrawContext;

	lct::spri::DrawContext* m_pSpriteDrawContext;
};

#endif//ASSET_VIEWER_MODE_H
