#ifndef ASSET_VIEWER_OVERLAY_H
#define ASSET_VIEWER_OVERLAY_H

#include <foun/foun_matrix.h>
#include <foun/foun_vector.h>
#include <foun/foun_callback.h>

#include <inpu/inpu_cursor.h>

#include <pack/pack_assetContainer.h>

#include <fill/fill_drawContext.h>

#include <font/font_assets.h>
#include <font/font_data.h>
#include <font/font_symbolBuffer.h>
#include <font/font_symbolWriter.h>
#include <font/font_drawContext.h>

#include <test/test_menu.h>
#include <test/test_menuPage.h>
#include <test/test_actionMenuItem.h>
#include <test/test_pageMenuItem.h>

#include <fram/fram_overlay.h>
#include <fram/fram_message.h>

class AssetViewerOverlay : public lct::fram::Overlay
{
public:
	AssetViewerOverlay();
	virtual ~AssetViewerOverlay();
	
	void SetInputCursor(lct::inpu::Cursor* pCursor);
	void SetSharedAssetContainer(lct::pack::AssetContainer* pAssetContainer);
	void SetFillDrawContext(lct::fill::DrawContext* pDrawContext);	
	void SetFontDrawContext(lct::font::DrawContext* pDrawContext);	

	virtual void Init();	

	virtual void AcquireGraphics();
	virtual void ReleaseGraphics();

	virtual void ReadInput();
	virtual void Update();
	virtual void Draw();

	void AddProgramOptionsMenuPage(lct::test::MenuPage* pMenuPage);

protected:
	// shared
	lct::inpu::Cursor* m_pInputCursor;
	lct::pack::AssetContainer* m_pSharedAssetContainer;
	lct::fill::DrawContext* m_pFillDrawContext;
	lct::font::DrawContext* m_pFontDrawContext;

	// owned	
	void OnCloseAction();

	lct::test::Menu m_menu;
	lct::test::MenuPage m_mainMenuPage;
	lct::test::PageMenuItem m_programOptionsMenuItem;
	lct::test::ActionMenuItem m_modeOptionsMenuItem;
	lct::test::ActionMenuItem m_closeMenuItem;

	lct::foun::Matrix44 m_projectionTransform;
};

#endif//ASSET_VIEWER_OVERLAY_H
