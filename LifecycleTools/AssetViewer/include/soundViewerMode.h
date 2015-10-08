#ifndef SOUND_VIEWER_MODE_H
#define SOUND_VIEWER_MODE_H

#include "assetViewerMode.h"

#include <foun/foun_linearAllocator.h>
#include <foun/foun_matrix.h>
#include <foun/foun_callback.h>

#include <soun/soun_assetHandler.h>
#include <soun/soun_assets.h>
#include <soun/soun_data.h>
#include <soun/soun_handles.h>
#include <soun/soun_stats.h>
// TEMP
#include <soun/soun_clipCoordinator.h>

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
	virtual void AcquireAudio();
	virtual void ReleaseAudio();

	virtual void ReadInput();
	virtual void Update();
	virtual void Draw();

	// TEMP!
	static const u32 CLIP_CAPACITY = 20;

private:
	void BuildMenu();
	void LoadAssets();

	lct::pack::AssetContainer m_assetContainer;
	lct::soun::AssetHandler m_soundAssetHandler;
	void* m_pAssetBinary;

	// TEMP!
	//lct::audi::WaveResource m_waveResource;	
	lct::soun::ClipCoordinator m_clipCoordinator;

	lct::soun::ClipHandle m_clipHandle;
	lct::soun::ClipStats m_clipStatsArray[CLIP_CAPACITY];

	void OnBeginClipTrigger();
	void OnEndClipTrigger();
	void OnReloadTrigger();

	const char** m_pClipNameArray;
	const char** m_pRampNameArray;

	lct::test::Menu m_menu;
	lct::test::MenuPage m_menuPage;
	lct::test::StringArrayMenuItem m_clipMenuItem;
	lct::test::StringArrayMenuItem m_rampMenuItem;
	lct::test::ActionMenuItem m_beginClipMenuItem;
	lct::test::ActionMenuItem m_endClipMenuItem;
	lct::test::ActionMenuItem m_reloadMenuItem;

	lct::foun::Matrix44 m_projectionTransform;
	lct::foun::Matrix32 m_contentWorldTransform;
};

#endif//SOUND_VIEWER_MODE_H
