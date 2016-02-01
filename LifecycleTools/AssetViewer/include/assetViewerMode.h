#ifndef ASSET_VIEWER_MODE_H
#define ASSET_VIEWER_MODE_H

#include <foun/foun_debug.h>

#include <file/file_accessor.h>

#include <inpu/inpu_cursor.h>

#include <pack/pack_assetContainer.h>

#include <fill/fill_drawContext.h>

#include <font/font_drawContext.h>

#include <spri/spri_drawContext.h>

#include <part/part_drawContext.h>

#include <fram/fram_mode.h>


class AssetViewerMode : public lct::fram::Mode
{
public:
	AssetViewerMode();
	virtual ~AssetViewerMode();

	struct SubShared
	{
		lct::file::Accessor* pAccessor;
		lct::inpu::Cursor* pCursor;
		lct::pack::AssetContainer* pAssetContainer;
		lct::fill::DrawContext* pFillDrawContext;
		lct::font::DrawContext* pFontDrawContext;
		lct::spri::DrawContext* pSpriteDrawContext;
		lct::part::DrawContext* pParticleDrawContext;
	};
	void SetSubShared(const SubShared& subShared);

	virtual void Init();

	virtual void AcquireGraphics();
	virtual void ReleaseGraphics();
	virtual void AcquireAudio();
	virtual void ReleaseAudio();

protected:
	SubShared m_subShared;
};

#endif//ASSET_VIEWER_MODE_H
