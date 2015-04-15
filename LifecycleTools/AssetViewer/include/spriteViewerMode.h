#ifndef DATA_MODE_H
#define DATA_MODE_H

#include "assetViewerMode.h"

#include <foun/foun_linearAllocator.h>
#include <foun/foun_matrix.h>
#include <foun/foun_callback.h>

namespace lct
{
namespace spri
{
	class AssetProcessor;
	class AssetContainer;
	struct FigureAsset;
	struct AnimationAsset;
	class BindingBuilder;
	struct AnimationFigureBinding;
	class InstanceBuilder;
	class FigureInstance;
	class AnimationInstance;
	class DrawContext;
}
namespace imag
{
	class AssetProcessor;
	class AssetContainer;
}
namespace font
{
	class SymbolBuffer;
	class SymbolWriter;
}
namespace test
{
	class ActionMenuItem;
	class StringArrayMenuItem;
	class FloatRangeMenuItem;
	class BoolMenuItem;
}
}

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

	lct::foun::LinearAllocator m_animationAllocator;

	lct::spri::AssetContainer* m_pSpriteAssetContainer;
	lct::spri::AssetProcessor* m_pSpriteAssetProcessor;
	lct::imag::AssetContainer* m_pImageAssetContainer;
	lct::imag::AssetProcessor* m_pImageAssetProcessor;
	void* m_pAssetBinary;
	lct::spri::FigureAsset* m_pFigureAsset;
	lct::spri::AnimationAsset* m_pAnimationAsset;
	lct::spri::BindingBuilder* m_pBindingBuilder;
	lct::spri::AnimationFigureBinding* m_pAnimationFigureBinding;
	lct::spri::InstanceBuilder* m_pInstanceBuilder;
	lct::spri::FigureInstance* m_pFigureInstance;
	lct::spri::AnimationInstance* m_pAnimationInstance;

	static const u32 SYMBOL_BUFFER_COUNT = 2;
	lct::font::SymbolBuffer* m_pSymbolBufferArray;
	u32 m_currSymbolBufferIndex;
	lct::font::SymbolWriter* m_pSymbolWriter;

	int OnAnimationCycle(int parameter);
	int OnStepCycle(float parameter);
	int OnFrameCycle(float parameter);
	int OnReloadTrigger(int parameter);

	lct::foun::InstanceCallback<SpriteViewerMode, int, int> m_animationCycleCallback;
	lct::foun::InstanceCallback<SpriteViewerMode, float, int> m_stepCycleCallback;
	lct::foun::InstanceCallback<SpriteViewerMode, float, int> m_frameCycleCallback;
	lct::foun::InstanceCallback<SpriteViewerMode, int, int> m_reloadTriggerCallback;

	const char** m_pAnimationNameArray;

	lct::test::Menu* m_pMenu;
	lct::test::StringArrayMenuItem* m_pAnimationMenuItem;
	lct::test::StringArrayMenuItem* m_pPlayMenuItem;
	lct::test::FloatRangeMenuItem* m_pStepMenuItem;
	lct::test::FloatRangeMenuItem* m_pFrameMenuItem;
	lct::test::BoolMenuItem* m_pDrawJointsMenuItem;
	lct::test::ActionMenuItem* m_pReloadMenuItem;

	lct::foun::Matrix44 m_projectionTransform;
	lct::foun::Matrix32 m_contentWorldTransform;

	//int OnBackTrigger(const int& parameter);

	//lct::foun::InstanceCallback<SpriteViewerOverlay, int, int> m_backTriggerCallback;
};

#endif//DATA_MODE_H
