#ifndef ASSET_VIEWER_OVERLAY_H
#define ASSET_VIEWER_OVERLAY_H

#include <inpu/inpu_cursor.h>

#include <pack/pack_assetContainer.h>

#include <fill/fill_drawContext.h>

#include <font/font_drawContext.h>

#include <fram/fram_overlay.h>

class AssetViewerOverlay : public lct::fram::Overlay
{
public:
	AssetViewerOverlay();
	virtual ~AssetViewerOverlay();

	struct SubShared
	{
		lct::inpu::Cursor* pCursor;
		lct::pack::AssetContainer* pAssetContainer;
		lct::fill::DrawContext* pFillDrawContext;
		lct::font::DrawContext* pFontDrawContext;
	};
	void SetSubShared(const SubShared& subShared);

protected:
	SubShared m_subShared;
};

#endif//ASSET_VIEWER_OVERLAY_H
