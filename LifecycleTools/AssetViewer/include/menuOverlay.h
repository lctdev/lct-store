#ifndef MENU_OVERLAY_H
#define MENU_OVERLAY_H

#include "assetViewerOverlay.h"

#include <foun/foun_matrix.h>
#include <foun/foun_vector.h>
#include <foun/foun_callback.h>

#include <font/font_assets.h>
#include <font/font_data.h>
#include <font/font_symbolBuffer.h>
#include <font/font_symbolWriter.h>

#include <test/test_menu.h>
#include <test/test_menuPage.h>
#include <test/test_actionMenuItem.h>
#include <test/test_pageMenuItem.h>

#include <fram/fram_message.h>

class MenuOverlay : public AssetViewerOverlay
{
public:
	MenuOverlay();
	virtual ~MenuOverlay();

	virtual void Init();

	virtual void AcquireGraphics();
	virtual void ReleaseGraphics();

	virtual bool DoesBlockInput();

	virtual void ReadInput();
	virtual void Update();
	virtual void Draw();

	void AddProgramOptionsMenuPage(lct::test::MenuPage* pMenuPage);

private:
	void OnCloseAction();

	lct::test::Menu m_menu;
	lct::test::MenuPage m_mainMenuPage;
	lct::test::PageMenuItem m_programOptionsMenuItem;
	lct::test::ActionMenuItem m_modeOptionsMenuItem;
	lct::test::ActionMenuItem m_closeMenuItem;

	lct::foun::Matrix44 m_projectionTransform;
};

#endif//MENU_OVERLAY_H
