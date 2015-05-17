#ifndef DATA_MODE_H
#define DATA_MODE_H

#include "assetViewerMode.h"

#include <foun/foun_linearAllocator.h>
#include <foun/foun_matrix.h>
#include <foun/foun_callback.h>

#include <spri/spri_assetProcessor.h>
#include <spri/spri_assetContainer.h>
#include <spri/spri_assets.h>
#include <spri/spri_data.h>
#include <spri/spri_bindingBuilder.h>
#include <spri/spri_bindings.h>
#include <spri/spri_instanceBuilder.h>
#include <spri/spri_figureInstance.h>
#include <spri/spri_animationInstance.h>

#include <imag/imag_assetProcessor.h>
#include <imag/imag_assetContainer.h>
#include <imag/imag_resourceHandler.h>

#include <pack/pack_packageWalker.h>

#include <util/util_binaryReader.h>

#include <file/file_accessor.h>

#include <font/font_assets.h>
#include <font/font_assetContainer.h>
#include <font/font_data.h>
#include <font/font_symbolBuffer.h>
#include <font/font_symbolWriter.h>
#include <font/font_drawContext.h>

#include <inpu/inpu_cursor.h>

#include <test/test_menu.h>
#include <test/test_actionMenuItem.h>
#include <test/test_stringArrayMenuItem.h>
#include <test/test_floatRangeMenuItem.h>
#include <test/test_boolMenuItem.h>


class SpriteViewerMode : public AssetViewerMode
{
public:
	SpriteViewerMode();
	virtual ~SpriteViewerMode();

	virtual void Init();

	virtual void AcquireGraphics();
	virtual void ReleaseGraphics();

	virtual void ReadInput();
	virtual void Update();
	virtual void Draw();

private:
	void BuildMenu();
	void LoadAssets();
	void BuildBindings();
	void BuildInstances();

	void BuildAnimation(const char* pName);

	// owned
	lct::foun::LinearAllocator m_animationAllocator;

	lct::spri::AssetContainer m_spriteAssetContainer;
	lct::spri::AssetProcessor m_spriteAssetProcessor;
	lct::imag::AssetContainer m_imageAssetContainer;
	lct::imag::AssetProcessor m_imageAssetProcessor;
	void* m_pAssetBinary;
	lct::spri::FigureAsset* m_pFigureAsset;
	lct::spri::AnimationAsset* m_pAnimationAsset;
	lct::spri::BindingBuilder m_bindingBuilder;
	lct::spri::AnimationFigureBinding* m_pAnimationFigureBinding;
	lct::spri::InstanceBuilder m_instanceBuilder;
	lct::spri::FigureInstance* m_pFigureInstance;
	lct::spri::AnimationInstance* m_pAnimationInstance;

	static const u32 SYMBOL_BUFFER_COUNT = 2;
	lct::font::SymbolBuffer m_symbolBufferArray[SYMBOL_BUFFER_COUNT];
	u32 m_currSymbolBufferIndex;
	lct::font::SymbolWriter m_symbolWriter;

	int OnAnimationCycle(int parameter);
	int OnStepCycle(float parameter);
	int OnFrameCycle(float parameter);
	int OnReloadTrigger(int parameter);

	lct::foun::InstanceCallback<SpriteViewerMode, int, int> m_animationCycleCallback;
	lct::foun::InstanceCallback<SpriteViewerMode, float, int> m_stepCycleCallback;
	lct::foun::InstanceCallback<SpriteViewerMode, float, int> m_frameCycleCallback;
	lct::foun::InstanceCallback<SpriteViewerMode, int, int> m_reloadTriggerCallback;

	const char** m_pAnimationNameArray;

	lct::test::Menu m_menu;
	lct::test::StringArrayMenuItem m_animationMenuItem;
	lct::test::StringArrayMenuItem m_playMenuItem;
	lct::test::FloatRangeMenuItem m_stepMenuItem;
	lct::test::FloatRangeMenuItem m_frameMenuItem;
	lct::test::BoolMenuItem m_drawJointsMenuItem;
	lct::test::ActionMenuItem m_reloadMenuItem;

	lct::foun::Matrix44 m_projectionTransform;
	lct::foun::Matrix32 m_contentWorldTransform;

	//int OnBackTrigger(const int& parameter);

	//lct::foun::InstanceCallback<SpriteViewerOverlay, int, int> m_backTriggerCallback;
};

#endif//DATA_MODE_H
