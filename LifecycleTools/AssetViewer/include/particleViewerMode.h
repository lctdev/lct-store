#ifndef PARTICLE_VIEWER_MODE_H
#define PARTICLE_VIEWER_MODE_H

#include "assetViewerMode.h"

#include <foun/foun_linearAllocator.h>
#include <foun/foun_matrix.h>
#include <foun/foun_callback.h>


//#include <spri/spri_assetHandler.h>
#include <part/part_assets.h>
#include <part/part_data.h>
#include <part/part_fieldInstance.h>


#include <imag/imag_assetHandler.h>

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


class ParticleViewerMode : public AssetViewerMode
{
public:
	ParticleViewerMode();
	virtual ~ParticleViewerMode();

	virtual void Init();

	virtual void AcquireGraphics();
	virtual void ReleaseGraphics();

	virtual void ReadInput();
	virtual void Update();
	virtual void Draw();

private:
	void BuildMenu();
	void LoadAssets();
	void BuildInstances();
	void BindField(const char* pName);

	lct::pack::AssetContainer m_assetContainer;
	//lct::spri::AssetHandler m_spriteAssetHandler;
	//lct::imag::AssetHandler m_imageAssetHandler;
	//void* m_pAssetBinary;
	lct::part::FieldAsset* m_pFieldAsset;
	lct::part::FieldInstance* m_pFieldInstance;

	void OnFieldCycle();
	void OnStepCycle();
	void OnFrameCycle();
	void OnReloadTrigger();

	const char** m_pFieldNameArray;

	lct::test::Menu m_menu;
	lct::test::MenuPage m_menuPage;
	lct::test::StringArrayMenuItem m_fieldMenuItem;
	lct::test::StringArrayMenuItem m_playMenuItem;
	lct::test::FloatRangeMenuItem m_stepMenuItem;
	lct::test::FloatRangeMenuItem m_frameMenuItem;
	//lct::test::BoolMenuItem m_drawJointsMenuItem;
	lct::test::ActionMenuItem m_reloadMenuItem;

	lct::foun::Matrix44 m_projectionTransform;
	lct::foun::Matrix32 m_contentWorldTransform;
};

#endif//PARTICLE_VIEWER_MODE_H
