#include "spriteViewerMode.h"
#include "assetViewerMessages.h"

#include <foun/foun_math.h>
#include <foun/foun_debug.h>

#include <string.h>

/*
 * Internal Constants
 */
static const u32 ANIMATION_MEMORY_SIZE = 1 * 1024 * 1024;

static const u32 SYMBOL_BUFFER_QUAD_COUNT = 512;

static const f32 MENU_OFFSET_X = 0.0f;
static const f32 MENU_OFFSET_Y = -100.0f;

static const f32 MENU_ITEM_LEFT_OFFSET_X = 160.0f;
static const f32 MENU_ITEM_MIDDLE_OFFSET_X = 280.0f;
static const f32 MENU_ITEM_RIGHT_OFFSET_X = 400.0f;

static const f32 AXIS_LINE_LENGTH = 512.0f;
static const f32 AXIS_LINE_THICKNESS = 1.0f;
static const lct::foun::FloatColor AXIS_LINE_COLOR = { 0.25f, 0.25f, 0.25f, 1.0f };

static const lct::foun::RectCentered JOINT_RECT = { 0.0f, 0.0f, 4.0f, 4.0f };
static const lct::foun::FloatColor JOINT_COLOR = { 0.0f, 1.0f, 0.0f, 1.0f };

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

, m_spriteAssetContainer()
, m_spriteAssetProcessor()
, m_imageAssetContainer()
, m_imageAssetProcessor()
, m_pAssetBinary(NULL)
, m_pFigureAsset(NULL)
, m_pAnimationAsset()
, m_bindingBuilder()
, m_pAnimationFigureBinding()
, m_instanceBuilder()
, m_pFigureInstance()
, m_pAnimationInstance()

, m_symbolBufferArray()
, m_currSymbolBufferIndex(0)
, m_symbolWriter()

, m_animationCycleCallback()
, m_stepCycleCallback()
, m_frameCycleCallback()
, m_reloadTriggerCallback()

, m_pAnimationNameArray(NULL)

, m_menu()
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
		void* pMemory = m_pAllocator->Alloc(ANIMATION_MEMORY_SIZE, 4);
		m_animationAllocator.SetMemory(pMemory, ANIMATION_MEMORY_SIZE);
	}

	m_spriteAssetContainer.SetAllocator(m_pAllocator);

	m_spriteAssetProcessor.SetAllocator(m_pAllocator);
	m_spriteAssetProcessor.SetAssetContainer(&m_spriteAssetContainer);

	m_imageAssetContainer.SetAllocator(m_pAllocator);

	m_imageAssetProcessor.SetAllocator(m_pAllocator);
	m_imageAssetProcessor.SetResourceHandler(m_pImageResourceHandler);
	m_imageAssetProcessor.SetAssetContainer(&m_imageAssetContainer);

	m_bindingBuilder.SetAllocator(m_pAllocator);

	m_instanceBuilder.SetAllocator(m_pAllocator);

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
	m_imageAssetProcessor.AcquireAllAssetResources();

	for (u32 bufferIndex = 0; bufferIndex < SYMBOL_BUFFER_COUNT; ++bufferIndex)
	{
		m_symbolBufferArray[bufferIndex].AcquireResources();
	}

	AssetViewerMode::AcquireGraphics();
}

void SpriteViewerMode::ReleaseGraphics()
{
	m_imageAssetProcessor.ReleaseAllAssetResources();

	for (u32 bufferIndex = 0; bufferIndex < SYMBOL_BUFFER_COUNT; ++bufferIndex)
	{
		m_symbolBufferArray[bufferIndex].ReleaseResources();
	}

	AssetViewerMode::ReleaseGraphics();
}

void SpriteViewerMode::ReadInput()
{
	lct::foun::Vector2 localCursorPosition;
	localCursorPosition.x = static_cast<f32>(m_pCursor->GetX()) - MENU_OFFSET_X;
	localCursorPosition.y = -static_cast<f32>(m_pCursor->GetY()) - MENU_OFFSET_Y;
	if (m_pCursor->IsPress())
	{
		m_menu.HandlePress(localCursorPosition);
	}
	if (m_pCursor->IsRelease())
	{
		m_menu.HandleRelease(localCursorPosition);
	}
}

void SpriteViewerMode::Update()
{
	const lct::foun::RectEdges& screenEdges = m_pScreen->GetRectEdges();
	lct::foun::Matrix44OrthographicProjection(m_projectionTransform, screenEdges.left, screenEdges.right, screenEdges.bottom, screenEdges.top, -1.0f, 1.0f);

	lct::foun::Matrix32Translate(m_contentWorldTransform, CONTENT_OFFSET_X, 0.0f);

	lct::font::SymbolBuffer& symbolBuffer = m_symbolBufferArray[m_currSymbolBufferIndex];
	symbolBuffer.ResetResources();
	m_symbolWriter.SetBuffer(&symbolBuffer);

	m_menu.WriteFont(&m_symbolWriter);
	symbolBuffer.UpdateResources();

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
	m_pFillDrawContext->ActivateRenderState();
	m_pFillDrawContext->ActivateShader();
	m_pFillDrawContext->ActivateQuad();
	m_pFillDrawContext->ActivateProjectionTransform(m_projectionTransform);

	lct::foun::RectCentered xAxisRect = { 0.0f, 0.0f, AXIS_LINE_LENGTH, AXIS_LINE_THICKNESS };
	lct::foun::RectCentered yAxisRect = { 0.0f, 0.0f, AXIS_LINE_THICKNESS, AXIS_LINE_LENGTH };
	m_pFillDrawContext->ActivateWorldTransform(m_contentWorldTransform);
	m_pFillDrawContext->DrawRect(xAxisRect, AXIS_LINE_COLOR);
	m_pFillDrawContext->DrawRect(yAxisRect, AXIS_LINE_COLOR);

	// sprite
	m_pFigureInstance->CalculateTextures();
	m_pFigureInstance->CalculateTransforms();

	m_pSpriteDrawContext->ActivateRenderState();
	m_pSpriteDrawContext->ActivateShader();
	m_pSpriteDrawContext->ActivateQuad();
	m_pSpriteDrawContext->ActivateProjectionTransform(m_projectionTransform);

	m_pSpriteDrawContext->ActivateWorldTransform(m_contentWorldTransform);
	m_pSpriteDrawContext->DrawFigure(*m_pFigureInstance);

	// sprite "indicators"
	m_pFillDrawContext->ActivateRenderState();
	m_pFillDrawContext->ActivateShader();
	m_pFillDrawContext->ActivateQuad();
	m_pFillDrawContext->ActivateProjectionTransform(m_projectionTransform);

	if (m_drawJointsMenuItem.GetValue())
	{
		// joints
		for (u32 jointIndex = 0; jointIndex < m_pFigureInstance->GetJointCount(); ++jointIndex)
		{
			const lct::foun::Matrix32& jointTransform = m_pFigureInstance->GetJointTransform(jointIndex);
			lct::foun::Matrix32 fillWorldTransform = jointTransform;
			fillWorldTransform.m[2][0] += CONTENT_OFFSET_X;
			m_pFillDrawContext->ActivateWorldTransform(fillWorldTransform);
			m_pFillDrawContext->DrawRect(JOINT_RECT, JOINT_COLOR);
		}
	}

	// menu
	// ( fill already active )
	const lct::foun::RectEdges& screenEdges = m_pScreen->GetRectEdges();
	lct::foun::Matrix32 menuWorldTransform;
	lct::foun::Matrix32Translate(menuWorldTransform, screenEdges.left + MENU_OFFSET_X, screenEdges.top + MENU_OFFSET_Y);
	m_pFillDrawContext->ActivateWorldTransform(menuWorldTransform);
	m_menu.DrawFill(m_pFillDrawContext);

	m_pFontDrawContext->ActivateRenderState();
	m_pFontDrawContext->ActivateShader();
	m_pFontDrawContext->ActivateProjectionTransform(m_projectionTransform);
	m_pFontDrawContext->ActivateWorldTransform(menuWorldTransform);

	lct::font::SymbolBuffer& symbolBuffer = m_symbolBufferArray[m_currSymbolBufferIndex];
	if (symbolBuffer.GetQuadCount() > 0)
	{
		m_pFontDrawContext->ActivateSymbolBuffer(symbolBuffer);
		m_pFontDrawContext->DrawSymbolBuffer(symbolBuffer);
	}
	m_currSymbolBufferIndex = 1 - m_currSymbolBufferIndex;
}

/*
 * Private Instance
 */
void SpriteViewerMode::BuildMenu()
{
	lct::font::SheetAsset* pSheetAsset = m_pFontAssetContainer->FindSheetAsset("example_sheet");

	for (u32 bufferIndex = 0; bufferIndex < SYMBOL_BUFFER_COUNT; ++bufferIndex)
	{
		lct::font::SymbolBuffer& symbolBuffer = m_symbolBufferArray[bufferIndex];
		symbolBuffer.SetAllocator(m_pAllocator);
		symbolBuffer.SetResourceHandler(m_pFontResourceHandler);
		symbolBuffer.SetSheetAsset(pSheetAsset);
		symbolBuffer.CreateResources(SYMBOL_BUFFER_QUAD_COUNT);
	}
	m_currSymbolBufferIndex = 0;

	m_animationCycleCallback.Bind(this, &SpriteViewerMode::OnAnimationCycle);
	m_stepCycleCallback.Bind(this, &SpriteViewerMode::OnStepCycle);
	m_frameCycleCallback.Bind(this, &SpriteViewerMode::OnFrameCycle);
	m_reloadTriggerCallback.Bind(this, &SpriteViewerMode::OnReloadTrigger);

	u32 animationNameCount = m_spriteAssetContainer.GetAnimationCount() + 1;
	m_pAnimationNameArray = m_pAllocator->AllocTypeArray<const char*>(animationNameCount);
	m_pAnimationNameArray[0] = "<NONE>";
	lct::spri::AssetContainer::AnimationIterator animationIterator = m_spriteAssetContainer.GetAnimationIterator();
	for (u32 animationNameIndex = 1; animationNameIndex < animationNameCount; ++animationNameIndex)
	{
		m_pAnimationNameArray[animationNameIndex] = animationIterator.GetKey().GetString();
		animationIterator.Next();
	}

	m_menu.SetSpacing(pSheetAsset->pSheetData->lineHeight * 2.0f);
	{
		lct::test::ActionMenuItem* pItem = m_pAllocator->AllocType<lct::test::ActionMenuItem>();
		pItem->SetLabel("Figure");
		m_menu.AddItem(pItem);
	}
	{
		m_animationMenuItem.SetLabel("Animation");
		m_animationMenuItem.SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X, MENU_ITEM_LEFT_OFFSET_X, MENU_ITEM_RIGHT_OFFSET_X);
		m_animationMenuItem.SetStringArray(m_pAnimationNameArray);
		m_animationMenuItem.SetCount(animationNameCount);
		m_animationMenuItem.SetIndex(0);
		m_animationMenuItem.SetCallback(&m_animationCycleCallback);
		m_menu.AddItem(&m_animationMenuItem);
	}
	{
		m_playMenuItem.SetLabel("Play");
		m_playMenuItem.SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X, MENU_ITEM_LEFT_OFFSET_X, MENU_ITEM_RIGHT_OFFSET_X);
		m_playMenuItem.SetStringArray(PLAY_TYPE_STRING_ARRAY);
		m_playMenuItem.SetCount(PLAY_TYPE_COUNT);
		m_playMenuItem.SetIndex(0);
		m_menu.AddItem(&m_playMenuItem);
	}
	{
		m_stepMenuItem.SetLabel("Step");
		m_stepMenuItem.SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X, MENU_ITEM_LEFT_OFFSET_X, MENU_ITEM_RIGHT_OFFSET_X);
		m_stepMenuItem.SetMin(0.0f);
		m_stepMenuItem.SetMax(8.0f);
		m_stepMenuItem.SetStep(0.25f);
		m_stepMenuItem.SetValue(1.0f);
		m_stepMenuItem.SetCallback(&m_stepCycleCallback);
		m_menu.AddItem(&m_stepMenuItem);
	}
	{
		m_frameMenuItem.SetLabel("Frame");
		m_frameMenuItem.SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X, MENU_ITEM_LEFT_OFFSET_X, MENU_ITEM_RIGHT_OFFSET_X);
		m_frameMenuItem.SetWrap(true);
		m_frameMenuItem.SetMin(0.0f);
		m_frameMenuItem.SetMax(0.0f);
		m_frameMenuItem.SetStep(1.0f);
		m_frameMenuItem.SetValue(0.0f);
		m_frameMenuItem.SetCallback(&m_frameCycleCallback);
		m_menu.AddItem(&m_frameMenuItem);
	}
	{
		m_drawJointsMenuItem.SetLabel("Draw Joints");
		m_drawJointsMenuItem.SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X, MENU_ITEM_LEFT_OFFSET_X, MENU_ITEM_RIGHT_OFFSET_X);
		m_drawJointsMenuItem.SetValue(false);
		m_menu.AddItem(&m_drawJointsMenuItem);
	}
	{
		m_reloadMenuItem.SetLabel("Reload");
		m_reloadMenuItem.SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X);
		m_reloadMenuItem.SetCallback(&m_reloadTriggerCallback);
		m_menu.AddItem(&m_reloadMenuItem);
	}
	m_menu.Arrange();
}

void SpriteViewerMode::LoadAssets()
{
	static const char* FILE_PATH = "data/spri/package.bin";
	u32 fileSize;
	m_pAssetBinary = m_pAccessor->LoadFile(FILE_PATH, &fileSize);
	lct::util::BinaryReader binaryReader;
	binaryReader.SetMemory(m_pAssetBinary, fileSize);
	lct::pack::PackageWalker packageWalker;
	packageWalker.AddAssetHandler(&m_spriteAssetProcessor);
	packageWalker.AddAssetHandler(&m_imageAssetProcessor);

	packageWalker.LoadAllAssets(binaryReader);

	m_spriteAssetProcessor.FixupAllAssets(m_imageAssetContainer);

	m_pFigureAsset = m_spriteAssetContainer.GetFigureIterator().GetValue();
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
	m_pAnimationAsset = m_spriteAssetContainer.FindAnimationAsset(pName);

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

int SpriteViewerMode::OnAnimationCycle(int parameter)
{
	const char* pAnimationName = m_pAnimationNameArray[parameter];
	BuildAnimation(pAnimationName);

	return 0;
}

int SpriteViewerMode::OnStepCycle(float parameter)
{
	// update dependent menu items
	f32 stepValue = m_stepMenuItem.GetValue();
	m_frameMenuItem.SetStep(stepValue);

	return 0;
}

int SpriteViewerMode::OnFrameCycle(float parameter)
{
	PlayType playType = static_cast<PlayType>(m_playMenuItem.GetIndex());
	if (playType == PLAY_TYPE_MANUAL)
	{
		f32 frameValue = m_frameMenuItem.GetValue();
		m_pAnimationInstance->ForceFrameOnTracks(frameValue);
	}

	return 0;
}

int SpriteViewerMode::OnReloadTrigger(int parameter)
{
	lct::fram::Message message;
	message.SetType(MESSAGE_TYPE_PROGRAM_CHANGE_MODE);
	ProgramChangeModeContent content;
	content.pNextModeName = "SpriteViewerMode";
	message.SetContent(&content);
	m_pProgramMessageQueue->EnqueueMessage(message);

	return 0;
}
