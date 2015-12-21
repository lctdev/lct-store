#include "spriteViewerMode.h"
#include "assetViewerMessages.h"

#include <foun/foun_math.h>
#include <foun/foun_debug.h>

#include <string.h>

/*
 * Internal Constants
 */
static const u32 ANIMATION_MEMORY_SIZE = 1 * 1024 * 1024;

static const f32 MENU_OFFSET_X = 0.0f;
static const f32 MENU_OFFSET_Y = -100.0f;

static const f32 MENU_ITEM_LEFT_OFFSET_X = 160.0f;
static const f32 MENU_ITEM_MIDDLE_OFFSET_X = 280.0f;
static const f32 MENU_ITEM_RIGHT_OFFSET_X = 400.0f;

static const f32 AXIS_LINE_LENGTH = 512.0f;
static const f32 AXIS_LINE_THICKNESS = 1.0f;
static const lct::foun::FloatColor4 AXIS_LINE_COLOR = { 0.25f, 0.25f, 0.25f, 1.0f };

static const lct::foun::RectCentered JOINT_RECT = { 0.0f, 0.0f, 4.0f, 4.0f };
static const lct::foun::FloatColor4 JOINT_COLOR = { 0.0f, 1.0f, 0.0f, 1.0f };

static const f32 CONTENT_OFFSET_X = 200.0f;

enum PlayType
{
	PLAY_TYPE_AUTOMATIC = 0,
	PLAY_TYPE_MANUAL,
	PLAY_TYPE_COUNT
};

static const char* PLAY_TYPE_STRING_ARRAY[PLAY_TYPE_COUNT] =
{
		"Automatic",
		"Manual"
};

/*
 * Public Instance
 */
SpriteViewerMode::SpriteViewerMode()
: AssetViewerMode()

, m_animationAllocator()

, m_assetContainer()
, m_spriteAssetHandler()
, m_imageAssetHandler()
, m_pAssetBinary(NULL)
, m_pFigureAsset(NULL)
, m_pAnimationAsset()
, m_bindingBuilder()
, m_pAnimationFigureBinding()
, m_instanceBuilder()
, m_pFigureInstance()
, m_pAnimationInstance()

, m_pAnimationNameArray(NULL)

, m_menu()
, m_menuPage()
, m_figureMenuItem()
, m_animationMenuItem()
, m_playMenuItem()
, m_stepMenuItem()
, m_frameMenuItem()
, m_drawJointsMenuItem()
, m_reloadMenuItem()

, m_projectionTransform()
, m_contentWorldTransform()
{
}

SpriteViewerMode::~SpriteViewerMode()
{
}

void SpriteViewerMode::Init()
{
	AssetViewerMode::Init();

	{
		void* pMemory = m_shared.pAllocator->Alloc(ANIMATION_MEMORY_SIZE, 4);
		m_animationAllocator.SetMemory(pMemory, ANIMATION_MEMORY_SIZE);
	}

	m_assetContainer.SetAllocator(m_shared.pAllocator);

	m_spriteAssetHandler.SetAllocator(m_shared.pAllocator);
	m_spriteAssetHandler.SetAssetContainer(&m_assetContainer);

	m_imageAssetHandler.SetAllocator(m_shared.pAllocator);
	m_imageAssetHandler.SetGraphicsDevice(m_shared.pGraphicsDevice);
	m_imageAssetHandler.SetAssetContainer(&m_assetContainer);

	m_bindingBuilder.SetAllocator(m_shared.pAllocator);

	m_instanceBuilder.SetAllocator(m_shared.pAllocator);

	LoadAssets();
	BuildMenu();
	BuildBindings();
	BuildInstances();

	const char* pAnimationName = m_pAnimationNameArray[0];
	BuildAnimation(pAnimationName);

	m_clearColor.r = 0.5f;
	m_clearColor.g = 0.5f;
	m_clearColor.b = 0.5f;
	m_clearColor.a = 1.0f;
}

void SpriteViewerMode::AcquireGraphics()
{
	m_imageAssetHandler.AcquireAllAssetResources();

	m_menu.AcquireGraphics();

	AssetViewerMode::AcquireGraphics();
}

void SpriteViewerMode::ReleaseGraphics()
{
	m_imageAssetHandler.ReleaseAllAssetResources();

	m_menu.ReleaseGraphics();

	AssetViewerMode::ReleaseGraphics();
}

void SpriteViewerMode::ReadInput()
{
	m_menu.HandleInput();
}

void SpriteViewerMode::Update()
{
	const lct::foun::RectEdges& screenEdges = m_shared.pScreen->GetRectEdges();
	lct::foun::Matrix44OrthographicProjection(m_projectionTransform, screenEdges.left, screenEdges.right, screenEdges.bottom, screenEdges.top, -1.0f, 1.0f);

	lct::foun::Matrix32Translate(m_contentWorldTransform, CONTENT_OFFSET_X, 0.0f);

	PlayType playType = static_cast<PlayType>(m_playMenuItem.GetIndex());
	if (playType == PLAY_TYPE_AUTOMATIC)
	{
		if (m_pAnimationInstance != NULL)
		{
			m_pAnimationInstance->UpdateTracks(m_stepMenuItem.GetValue());
		}

		// update dependent menu items
		f32 frameValue = m_frameMenuItem.GetValue();
		frameValue += m_stepMenuItem.GetValue();
		f32 frameMax = m_frameMenuItem.GetMax();
		if (frameMax == 0.0f)
		{
			frameValue = 0.0f;
		}
		else
		{
			while (frameValue > frameMax)
			{
				frameValue -= frameMax;
			}
		}
		m_frameMenuItem.SetValue(frameValue);
	}
}

void SpriteViewerMode::Draw()
{
	// axes
	m_subShared.pFillDrawContext->ActivateRenderState();
	m_subShared.pFillDrawContext->ActivateShader();
	m_subShared.pFillDrawContext->ActivateQuad();
	m_subShared.pFillDrawContext->ActivateProjectionTransform(m_projectionTransform);

	lct::foun::RectCentered xAxisRect = { 0.0f, 0.0f, AXIS_LINE_LENGTH, AXIS_LINE_THICKNESS };
	lct::foun::RectCentered yAxisRect = { 0.0f, 0.0f, AXIS_LINE_THICKNESS, AXIS_LINE_LENGTH };
	m_subShared.pFillDrawContext->ActivateWorldTransform(m_contentWorldTransform);
	m_subShared.pFillDrawContext->DrawRect(xAxisRect, AXIS_LINE_COLOR);
	m_subShared.pFillDrawContext->DrawRect(yAxisRect, AXIS_LINE_COLOR);

	m_subShared.pFillDrawContext->DeactivateQuad();

	// sprite
	m_pFigureInstance->CalculateTextures();
	m_pFigureInstance->CalculateTransforms();

	m_subShared.pSpriteDrawContext->ActivateRenderState();
	m_subShared.pSpriteDrawContext->ActivateShader();
	m_subShared.pSpriteDrawContext->ActivateQuad();
	m_subShared.pSpriteDrawContext->ActivateProjectionTransform(m_projectionTransform);

	m_subShared.pSpriteDrawContext->ActivateWorldTransform(m_contentWorldTransform);
	m_subShared.pSpriteDrawContext->DrawFigure(*m_pFigureInstance);

	m_subShared.pSpriteDrawContext->DeactivateQuad();

	// sprite "indicators"
	m_subShared.pFillDrawContext->ActivateRenderState();
	m_subShared.pFillDrawContext->ActivateShader();
	m_subShared.pFillDrawContext->ActivateQuad();
	m_subShared.pFillDrawContext->ActivateProjectionTransform(m_projectionTransform);

	if (m_drawJointsMenuItem.GetValue())
	{
		// joints
		for (u32 jointIndex = 0; jointIndex < m_pFigureInstance->GetJointCount(); ++jointIndex)
		{
			const lct::foun::Matrix32& jointTransform = m_pFigureInstance->GetJointTransform(jointIndex);
			lct::foun::Matrix32 fillWorldTransform = jointTransform;
			fillWorldTransform.m[2][0] += CONTENT_OFFSET_X;
			m_subShared.pFillDrawContext->ActivateWorldTransform(fillWorldTransform);

			m_subShared.pFillDrawContext->DrawRect(JOINT_RECT, JOINT_COLOR);
		}
	}

	m_subShared.pFillDrawContext->DeactivateQuad();

	// menu
	m_menu.Draw();
}

/*
 * Private Instance
 */
void SpriteViewerMode::BuildMenu()
{
	lct::font::SheetAsset* pSheetAsset = m_subShared.pAssetContainer->FindAsset<lct::font::SheetAsset>("example_sheet");

	lct::test::Menu::Shared menuShared;
	menuShared.pAllocator = m_shared.pAllocator;
	menuShared.pScreen = m_shared.pScreen;
	menuShared.pGraphicsDevice = m_shared.pGraphicsDevice;
	menuShared.pInputCursor = m_subShared.pCursor;
	menuShared.pFillDrawContext = m_subShared.pFillDrawContext;
	menuShared.pFontDrawContext = m_subShared.pFontDrawContext;
	menuShared.pSheetAsset = pSheetAsset;
	m_menu.SetShared(menuShared);
	m_menu.Initialize();

	u32 animationNameCount = m_assetContainer.GetAssetCount<lct::spri::AnimationAsset>() + 1;
	m_pAnimationNameArray = m_shared.pAllocator->AllocTypeArray<const char*>(animationNameCount);
	m_pAnimationNameArray[0] = "<NONE>";
	lct::pack::AssetIterator<lct::spri::AnimationAsset> animationIterator = m_assetContainer.GetIterator<lct::spri::AnimationAsset>();
	for (u32 animationNameIndex = 1; animationNameIndex < animationNameCount; ++animationNameIndex)
	{
		m_pAnimationNameArray[animationNameIndex] = animationIterator.GetAsset()->pAnimationData->name;
		animationIterator.Next();
	}

	m_menu.SetSpacing(pSheetAsset->pSheetData->lineHeight * 2.0f);
	lct::foun::Vector2 menuPosition = { MENU_OFFSET_X, MENU_OFFSET_Y };
	m_menu.SetPosition(menuPosition);

	m_menuPage.SetLabel("Default");
	{
		// TEMP!
		m_figureMenuItem.SetLabel("Figure");
		m_reloadMenuItem.SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X);
		m_menuPage.AddItem(&m_figureMenuItem);
	}
	{
		m_animationMenuItem.SetLabel("Animation");
		m_animationMenuItem.SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X, MENU_ITEM_LEFT_OFFSET_X, MENU_ITEM_RIGHT_OFFSET_X);
		m_animationMenuItem.SetStringArray(m_pAnimationNameArray);
		m_animationMenuItem.SetCount(animationNameCount);
		m_animationMenuItem.SetIndex(0);
		m_animationMenuItem.GetCycleCallback().Bind(this, &SpriteViewerMode::OnAnimationCycle);
		m_menuPage.AddItem(&m_animationMenuItem);
	}
	{
		m_playMenuItem.SetLabel("Play");
		m_playMenuItem.SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X, MENU_ITEM_LEFT_OFFSET_X, MENU_ITEM_RIGHT_OFFSET_X);
		m_playMenuItem.SetStringArray(PLAY_TYPE_STRING_ARRAY);
		m_playMenuItem.SetCount(PLAY_TYPE_COUNT);
		m_playMenuItem.SetIndex(0);
		m_menuPage.AddItem(&m_playMenuItem);
	}
	{
		m_stepMenuItem.SetLabel("Step");
		m_stepMenuItem.SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X, MENU_ITEM_LEFT_OFFSET_X, MENU_ITEM_RIGHT_OFFSET_X);
		m_stepMenuItem.SetMin(0.0f);
		m_stepMenuItem.SetMax(8.0f);
		m_stepMenuItem.SetStep(0.25f);
		m_stepMenuItem.SetValue(1.0f);
		m_stepMenuItem.GetCycleCallback().Bind(this, &SpriteViewerMode::OnStepCycle);
		m_menuPage.AddItem(&m_stepMenuItem);
	}
	{
		m_frameMenuItem.SetLabel("Frame");
		m_frameMenuItem.SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X, MENU_ITEM_LEFT_OFFSET_X, MENU_ITEM_RIGHT_OFFSET_X);
		m_frameMenuItem.SetWrap(true);
		m_frameMenuItem.SetMin(0.0f);
		m_frameMenuItem.SetMax(0.0f);
		m_frameMenuItem.SetStep(1.0f);
		m_frameMenuItem.SetValue(0.0f);
		m_frameMenuItem.GetCycleCallback().Bind(this, &SpriteViewerMode::OnFrameCycle);
		m_menuPage.AddItem(&m_frameMenuItem);
	}
	{
		m_drawJointsMenuItem.SetLabel("Draw Joints");
		m_drawJointsMenuItem.SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X, MENU_ITEM_LEFT_OFFSET_X, MENU_ITEM_RIGHT_OFFSET_X);
		m_drawJointsMenuItem.SetValue(false);
		m_menuPage.AddItem(&m_drawJointsMenuItem);
	}
	{
		m_reloadMenuItem.SetLabel("Reload");
		m_reloadMenuItem.SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X);
		m_reloadMenuItem.GetCallback().Bind(this, &SpriteViewerMode::OnReloadTrigger);
		m_menuPage.AddItem(&m_reloadMenuItem);
	}
	m_menu.AddPage(&m_menuPage);
	m_menu.Arrange();
	m_menu.ActivatePage("Default");
}

void SpriteViewerMode::LoadAssets()
{
	static const char* FILE_PATH = "data/spriteViewer/package.bin";
	ssiz fileSize;
	m_pAssetBinary = m_subShared.pAccessor->LoadFile(FILE_PATH, &fileSize);
	lct::util::BinaryReader binaryReader;
	binaryReader.SetMemory(m_pAssetBinary, fileSize);
	lct::pack::PackageWalker packageWalker;
	packageWalker.AddAssetHandler(&m_spriteAssetHandler);
	packageWalker.AddAssetHandler(&m_imageAssetHandler);

	packageWalker.LoadAllAssets(binaryReader);

	m_spriteAssetHandler.FixupAllAssets();

	m_pFigureAsset = m_assetContainer.GetIterator<lct::spri::FigureAsset>().GetAsset();
}

void SpriteViewerMode::BuildBindings()
{

}

void SpriteViewerMode::BuildInstances()
{
	m_pFigureInstance = m_instanceBuilder.CreateFigureInstance(*m_pFigureAsset);

	/*m_pAnimationInstance = m_pInstanceBuilder->CreateAnimationInstance(m_pAnimationAsset->pAnimationData->trackCount);
	m_pAnimationInstance->BindFigureInstance(*m_pFigureInstance);
	m_pAnimationInstance->BindAnimationAsset(*m_pAnimationAsset, *m_pAnimationFigureBinding);*/
}

void SpriteViewerMode::BuildAnimation(const char* pName)
{
	// reset all properties on the figure
	m_pFigureInstance->ResetProperties();

	// null any old animation objects
	m_pAnimationFigureBinding = NULL;
	m_pAnimationInstance = NULL;

	// clear any old allocations
	m_animationAllocator.Clear();

	// find the specified asset
	m_pAnimationAsset = m_assetContainer.FindAsset<lct::spri::AnimationAsset>(pName);

	f32 frameMax = 0.0f;
	if (m_pAnimationAsset != NULL)
	{
		// create the binding
		m_bindingBuilder.SetAllocator(&m_animationAllocator);
		m_pAnimationFigureBinding = m_bindingBuilder.CreateAnimationFigureBinding(*m_pAnimationAsset, *m_pFigureAsset);

		// create the instance and bind to the figure
		m_instanceBuilder.SetAllocator(&m_animationAllocator);
		m_pAnimationInstance = m_instanceBuilder.CreateAnimationInstance(m_pAnimationAsset->pAnimationData->trackCount);
		m_pAnimationInstance->BindFigureInstance(*m_pFigureInstance);
		m_pAnimationInstance->BindAnimationAsset(*m_pAnimationAsset, *m_pAnimationFigureBinding);

		frameMax = m_pAnimationInstance->FindLongestTrackFrameDuration();
	}

	// update dependent menu items
	m_frameMenuItem.SetMax(frameMax);
	m_frameMenuItem.SetValue(0.0f);
}

void SpriteViewerMode::OnAnimationCycle()
{
	u32 animationIndex = m_animationMenuItem.GetIndex();
	const char* pAnimationName = m_pAnimationNameArray[animationIndex];
	BuildAnimation(pAnimationName);
}

void SpriteViewerMode::OnStepCycle()
{
	// update dependent menu items
	f32 stepValue = m_stepMenuItem.GetValue();
	m_frameMenuItem.SetStep(stepValue);
}

void SpriteViewerMode::OnFrameCycle()
{
	PlayType playType = static_cast<PlayType>(m_playMenuItem.GetIndex());
	if (playType == PLAY_TYPE_MANUAL)
	{
		f32 frameValue = m_frameMenuItem.GetValue();
		m_pAnimationInstance->ForceFrameOnTracks(frameValue);
	}
}

void SpriteViewerMode::OnReloadTrigger()
{
	lct::fram::Message message;
	message.SetType(MESSAGE_TYPE_PROGRAM_CHANGE_MODE);
	ProgramChangeModeContent content;
	content.pNextModeName = "SpriteViewerMode";
	message.SetContent(&content);
	m_shared.pProgramMessageQueue->EnqueueMessage(message);
}
