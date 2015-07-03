#ifndef ASSET_VIEWER_MODE_H
#define ASSET_VIEWER_MODE_H

#include <foun/foun_debug.h>

#include <file/file_accessor.h>

#include <inpu/inpu_cursor.h>

#include <pack/pack_assetContainer.h>

#include <fill/fill_drawContext.h>

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
	void SetSharedAssetContainer(lct::pack::AssetContainer* pAssetContainer);
	void SetFillDrawContext(lct::fill::DrawContext* pDrawContext);	
	void SetFontDrawContext(lct::font::DrawContext* pDrawContext);
	void SetSpriteDrawContext(lct::spri::DrawContext* pDrawContext);

	virtual void Init();

	virtual void AcquireGraphics();
	virtual void ReleaseGraphics();

protected:
	// shared
	lct::file::Accessor* m_pAccessor;

	lct::inpu::Cursor* m_pCursor;

	lct::pack::AssetContainer* m_pSharedAssetContainer;

	lct::fill::DrawContext* m_pFillDrawContext;
	
	lct::font::DrawContext* m_pFontDrawContext;

	lct::spri::DrawContext* m_pSpriteDrawContext;
};

#endif//ASSET_VIEWER_MODE_H
