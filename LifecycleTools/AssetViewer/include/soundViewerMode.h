#ifndef SOUND_VIEWER_MODE_H
#define SOUND_VIEWER_MODE_H

#include "assetViewerMode.h"

#include <foun/foun_linearAllocator.h>
#include <foun/foun_matrix.h>
#include <foun/foun_callback.h>

#include <pack/pack_packageWalker.h>
#include <pack/pack_assetContainer.h>

#include <util/util_binaryReader.h>

#include <file/file_accessor.h>

#include <font/font_assets.h>
#include <font/font_data.h>
#include <font/font_symbolBuffer.h>
#include <font/font_symbolWriter.h>
#include <font/font_drawContext.h>

#include <inpu/inpu_cursor.h>

#include <test/test_menu.h>
#include <test/test_menuPage.h>
#include <test/test_actionMenuItem.h>
#include <test/test_stringArrayMenuItem.h>
#include <test/test_floatRangeMenuItem.h>
#include <test/test_boolMenuItem.h>


class SoundViewerMode : public AssetViewerMode
{
public:
	SoundViewerMode();
	virtual ~SoundViewerMode();

	virtual void Init();

	virtual void AcquireGraphics();
	virtual void ReleaseGraphics();

	virtual void ReadInput();
	virtual void Update();
	virtual void Draw();

private:
	void BuildMenu();
	void LoadAssets();

	lct::pack::AssetContainer m_assetContainer;

	// TEMP!
	lct::audi::WaveResource m_waveResource;

	void OnReloadTrigger();

	lct::test::Menu m_menu;
	lct::test::MenuPage m_menuPage;
	lct::test::ActionMenuItem m_reloadMenuItem;

	lct::foun::Matrix44 m_projectionTransform;
	lct::foun::Matrix32 m_contentWorldTransform;
};

#endif//SOUND_VIEWER_MODE_H
