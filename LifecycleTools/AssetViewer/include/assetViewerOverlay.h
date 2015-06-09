#ifndef ASSET_VIEWER_OVERLAY_H
#define ASSET_VIEWER_OVERLAY_H

#include <foun/foun_matrix.h>
#include <foun/foun_vector.h>
#include <foun/foun_callback.h>

#include <inpu/inpu_cursor.h>

#include <fill/fill_drawContext.h>

#include <font/font_assets.h>
#include <font/font_assetContainer.h>
#include <font/font_data.h>
#include <font/font_symbolBuffer.h>
#include <font/font_symbolWriter.h>
#include <font/font_drawContext.h>

#include <test/test_menu.h>
#include <test/test_actionMenuItem.h>

#include <fram/fram_overlay.h>
#include <fram/fram_message.h>

class SpriteViewerOverlay : public lct::fram::Overlay
{
public:
	SpriteViewerOverlay();
	virtual ~SpriteViewerOverlay();
	
	void SetInputCursor(lct::inpu::Cursor* pCursor);
	void SetFillDrawContext(lct::fill::DrawContext* pDrawContext);
	void SetFontAssetContainer(lct::font::AssetContainer* pAssetContainer);
	void SetFontDrawContext(lct::font::DrawContext* pDrawContext);

	virtual void Init();

	virtual void AcquireGraphics();
	virtual void ReleaseGraphics();

	virtual void ReadInput();
	virtual void Update();
	virtual void Draw();

protected:
	// shared
	lct::inpu::Cursor* m_pInputCursor;

	lct::fill::DrawContext* m_pFillDrawContext;

	lct::font::AssetContainer* m_pFontAssetContainer;
	lct::font::DrawContext* m_pFontDrawContext;

	// owned
	static const u32 SYMBOL_BUFFER_COUNT = 2;
	lct::font::SymbolBuffer m_symbolBufferArray[SYMBOL_BUFFER_COUNT];
	u32 m_currSymbolBufferIndex;
	lct::font::SymbolWriter m_symbolWriter;	

	int OnBackAction(int parameter);

	lct::foun::InstanceCallback<SpriteViewerOverlay, int, int> m_backActionCallback;

	lct::test::Menu m_menu;

	lct::foun::Matrix44 m_projectionTransform;
};

#endif//ASSET_VIEWER_OVERLAY_H
