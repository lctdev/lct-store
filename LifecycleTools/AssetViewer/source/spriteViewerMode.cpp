#include "spriteViewerMode.h"
#include "assetViewerMessages.h"

#include <spri/spri_assetProcessor.h>
#include <spri/spri_assetContainer.h>
#include <spri/spri_assets.h>
#include <spri/spri_data.h>
#include <spri/spri_bindingBuilder.h>
#include <spri/spri_bindings.h>
#include <spri/spri_instanceBuilder.h>
#include <spri/spri_figureInstance.h>
#include <spri/spri_animationInstance.h>
#include <spri/spri_drawContext.h>

#include <imag/imag_assetProcessor.h>
#include <imag/imag_assetContainer.h>
#include <imag/imag_resourceHandler.h>

#include <pack/pack_packageWalker.h>

#include <util/util_binaryReader.h>

#include <foun/foun_math.h>
#include <foun/foun_debug.h>

#include <file/file_accessor.h>

#include <fram/fram_screen.h>
#include <fram/fram_messageQueue.h>
#include <fram/fram_message.h>

#include <fill/fill_drawContext.h>

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

, m_pSpriteAssetContainer(NULL)
, m_pSpriteAssetProcessor(NULL)
, m_pImageAssetContainer(NULL)
, m_pImageAssetProcessor(NULL)
, m_pAssetBinary(NULL)
, m_pFigureAsset(NULL)
, m_pAnimationAsset(NULL)
, m_pBindingBuilder(NULL)
, m_pAnimationFigureBinding(NULL)
, m_pInstanceBuilder(NULL)
, m_pFigureInstance(NULL)
, m_pAnimationInstance(NULL)

, m_pSymbolBufferArray(NULL)
, m_currSymbolBufferIndex(0)
, m_pSymbolWriter(NULL)

, m_animationCycleCallback()
, m_stepCycleCallback()
, m_frameCycleCallback()
, m_reloadTriggerCallback()

, m_pAnimationNameArray(NULL)

, m_pMenu(NULL)
, m_pAnimationMenuItem(NULL)
, m_pPlayMenuItem(NULL)
, m_pStepMenuItem(NULL)
, m_pFrameMenuItem(NULL)
, m_pDrawJointsMenuItem(NULL)
, m_pReloadMenuItem(NULL)

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

	m_pSpriteAssetContainer = m_pAllocator->AllocType<lct::spri::AssetContainer>();
	m_pSpriteAssetContainer->SetAllocator(m_pAllocator);

	m_pSpriteAssetProcessor = m_pAllocator->AllocType<lct::spri::AssetProcessor>();
	m_pSpriteAssetProcessor->SetAllocator(m_pAllocator);
	m_pSpriteAssetProcessor->SetAssetContainer(m_pSpriteAssetContainer);

	m_pImageAssetContainer = m_pAllocator->AllocType<lct::imag::AssetContainer>();
	m_pImageAssetContainer->SetAllocator(m_pAllocator);

	m_pImageAssetProcessor = m_pAllocator->AllocType<lct::imag::AssetProcessor>();
	m_pImageAssetProcessor->SetAllocator(m_pAllocator);
	m_pImageAssetProcessor->SetResourceHandler(m_pImageResourceHandler);
	m_pImageAssetProcessor->SetAssetContainer(m_pImageAssetContainer);

	m_pBindingBuilder = m_pAllocator->AllocType<lct::spri::BindingBuilder>();
	m_pBindingBuilder->SetAllocator(m_pAllocator);

	m_pInstanceBuilder = m_pAllocator->AllocType<lct::spri::InstanceBuilder>();
	m_pInstanceBuilder->SetAllocator(m_pAllocator);

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
	m_pImageAssetProcessor->AcquireAllAssetResources();

	for (u32 bufferIndex = 0; bufferIndex < SYMBOL_BUFFER_COUNT; ++bufferIndex)
	{
		m_pSymbolBufferArray[bufferIndex].AcquireResources();
	}

	AssetViewerMode::AcquireGraphics();
}

void SpriteViewerMode::ReleaseGraphics()
{
	m_pImageAssetProcessor->ReleaseAllAssetResources();

	for (u32 bufferIndex = 0; bufferIndex < SYMBOL_BUFFER_COUNT; ++bufferIndex)
	{
		m_pSymbolBufferArray[bufferIndex].ReleaseResources();
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
		m_pMenu->HandlePress(localCursorPosition);
	}
	if (m_pCursor->IsRelease())
	{
		m_pMenu->HandleRelease(localCursorPosition);
	}
}

void SpriteViewerMode::Update()
{
	const lct::foun::RectEdges& screenEdges = m_pScreen->GetRectEdges();
	lct::foun::Matrix44OrthographicProjection(m_projectionTransform, screenEdges.left, screenEdges.right, screenEdges.bottom, screenEdges.top, -1.0f, 1.0f);

	lct::foun::Matrix32Translate(m_contentWorldTransform, CONTENT_OFFSET_X, 0.0f);

	lct::font::SymbolBuffer& symbolBuffer = m_pSymbolBufferArray[m_currSymbolBufferIndex];
	symbolBuffer.ResetResources();
	m_pSymbolWriter->SetBuffer(&symbolBuffer);

	m_pMenu->WriteFont(m_pSymbolWriter);
	symbolBuffer.UpdateResources();

	PlayType playType = static_cast<PlayType>(m_pPlayMenuItem->GetIndex());
	if (playType == PLAY_TYPE_AUTOMATIC)
	{
		if (m_pAnimationInstance != NULL)
		{
			m_pAnimationInstance->UpdateTracks(m_pStepMenuItem->GetValue());
		}

		// update dependent menu items
		f32 frameValue = m_pFrameMenuItem->GetValue();
		frameValue += m_pStepMenuItem->GetValue();
		f32 frameMax = m_pFrameMenuItem->GetMax();
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
		m_pFrameMenuItem->SetValue(frameValue);
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

	if (m_pDrawJointsMenuItem->GetValue())
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
	m_pMenu->DrawFill(m_pFillDrawContext);

	m_pFontDrawContext->ActivateRenderState();
	m_pFontDrawContext->ActivateShader();
	m_pFontDrawContext->ActivateProjectionTransform(m_projectionTransform);
	m_pFontDrawContext->ActivateWorldTransform(menuWorldTransform);

	lct::font::SymbolBuffer& symbolBuffer = m_pSymbolBufferArray[m_currSymbolBufferIndex];
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

	m_pSymbolBufferArray = m_pAllocator->AllocTypeArray<lct::font::SymbolBuffer>(SYMBOL_BUFFER_COUNT);
	for (u32 bufferIndex = 0; bufferIndex < SYMBOL_BUFFER_COUNT; ++bufferIndex)
	{
		lct::font::SymbolBuffer& symbolBuffer = m_pSymbolBufferArray[bufferIndex];
		symbolBuffer.SetAllocator(m_pAllocator);
		symbolBuffer.SetResourceHandler(m_pFontResourceHandler);
		symbolBuffer.SetSheetAsset(pSheetAsset);
		symbolBuffer.CreateResources(SYMBOL_BUFFER_QUAD_COUNT);
	}
	m_currSymbolBufferIndex = 0;

	m_pSymbolWriter = m_pAllocator->AllocType<lct::font::SymbolWriter>();

	m_animationCycleCallback.Bind(this, &SpriteViewerMode::OnAnimationCycle);
	m_stepCycleCallback.Bind(this, &SpriteViewerMode::OnStepCycle);
	m_frameCycleCallback.Bind(this, &SpriteViewerMode::OnFrameCycle);
	m_reloadTriggerCallback.Bind(this, &SpriteViewerMode::OnReloadTrigger);

	u32 animationNameCount = m_pSpriteAssetContainer->GetAnimationCount() + 1;
	m_pAnimationNameArray = m_pAllocator->AllocTypeArray<const char*>(animationNameCount);
	m_pAnimationNameArray[0] = "<NONE>";
	lct::spri::AssetContainer::AnimationIterator animationIterator = m_pSpriteAssetContainer->GetAnimationIterator();
	for (u32 animationNameIndex = 1; animationNameIndex < animationNameCount; ++animationNameIndex)
	{
		m_pAnimationNameArray[animationNameIndex] = animationIterator.GetKey().GetString();
		animationIterator.Next();
	}

	m_pMenu = m_pAllocator->AllocType<lct::test::Menu>();
	m_pMenu->SetSpacing(pSheetAsset->pSheetData->lineHeight * 2.0f);
	{
		lct::test::ActionMenuItem* pItem = m_pAllocator->AllocType<lct::test::ActionMenuItem>();
		pItem->SetLabel("Figure");
		m_pMenu->AddItem(pItem);
	}
	{
		m_pAnimationMenuItem = m_pAllocator->AllocType<lct::test::StringArrayMenuItem>();
		m_pAnimationMenuItem->SetLabel("Animation");
		m_pAnimationMenuItem->SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X, MENU_ITEM_LEFT_OFFSET_X, MENU_ITEM_RIGHT_OFFSET_X);
		m_pAnimationMenuItem->SetStringArray(m_pAnimationNameArray);
		m_pAnimationMenuItem->SetCount(animationNameCount);
		m_pAnimationMenuItem->SetIndex(0);
		m_pAnimationMenuItem->SetCallback(&m_animationCycleCallback);
		m_pMenu->AddItem(m_pAnimationMenuItem);
	}
	{
		m_pPlayMenuItem = m_pAllocator->AllocType<lct::test::StringArrayMenuItem>();
		m_pPlayMenuItem->SetLabel("Play");
		m_pPlayMenuItem->SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X, MENU_ITEM_LEFT_OFFSET_X, MENU_ITEM_RIGHT_OFFSET_X);
		m_pPlayMenuItem->SetStringArray(PLAY_TYPE_STRING_ARRAY);
		m_pPlayMenuItem->SetCount(PLAY_TYPE_COUNT);
		m_pPlayMenuItem->SetIndex(0);
		m_pMenu->AddItem(m_pPlayMenuItem);
	}
	{
		m_pStepMenuItem = m_pAllocator->AllocType<lct::test::FloatRangeMenuItem>();
		m_pStepMenuItem->SetLabel("Step");
		m_pStepMenuItem->SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X, MENU_ITEM_LEFT_OFFSET_X, MENU_ITEM_RIGHT_OFFSET_X);
		m_pStepMenuItem->SetMin(0.0f);
		m_pStepMenuItem->SetMax(8.0f);
		m_pStepMenuItem->SetStep(0.25f);
		m_pStepMenuItem->SetValue(1.0f);
		m_pStepMenuItem->SetCallback(&m_stepCycleCallback);
		m_pMenu->AddItem(m_pStepMenuItem);
	}
	{
		m_pFrameMenuItem = m_pAllocator->AllocType<lct::test::FloatRangeMenuItem>();
		m_pFrameMenuItem->SetLabel("Frame");
		m_pFrameMenuItem->SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X, MENU_ITEM_LEFT_OFFSET_X, MENU_ITEM_RIGHT_OFFSET_X);
		m_pFrameMenuItem->SetWrap(true);
		m_pFrameMenuItem->SetMin(0.0f);
		m_pFrameMenuItem->SetMax(0.0f);
		m_pFrameMenuItem->SetStep(1.0f);
		m_pFrameMenuItem->SetValue(0.0f);
		m_pFrameMenuItem->SetCallback(&m_frameCycleCallback);
		m_pMenu->AddItem(m_pFrameMenuItem);
	}
	{
		m_pDrawJointsMenuItem = m_pAllocator->AllocType<lct::test::BoolMenuItem>();
		m_pDrawJointsMenuItem->SetLabel("Draw Joints");
		m_pDrawJointsMenuItem->SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X, MENU_ITEM_LEFT_OFFSET_X, MENU_ITEM_RIGHT_OFFSET_X);
		m_pDrawJointsMenuItem->SetValue(false);
		m_pMenu->AddItem(m_pDrawJointsMenuItem);
	}
	{
		m_pReloadMenuItem = m_pAllocator->AllocType<lct::test::ActionMenuItem>();
		m_pReloadMenuItem->SetLabel("Reload");
		m_pReloadMenuItem->SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X);
		m_pReloadMenuItem->SetCallback(&m_reloadTriggerCallback);
		m_pMenu->AddItem(m_pReloadMenuItem);
	}
	m_pMenu->Arrange();
}

void SpriteViewerMode::LoadAssets()
{
	static const char* FILE_PATH = "data/spri/package.bin";
	u32 fileSize;
	m_pAssetBinary = m_pAccessor->LoadFile(FILE_PATH, &fileSize);
	lct::util::BinaryReader binaryReader;
	binaryReader.SetMemory(m_pAssetBinary, fileSize);
	lct::pack::PackageWalker packageWalker;
	packageWalker.AddAssetHandler(m_pSpriteAssetProcessor);
	packageWalker.AddAssetHandler(m_pImageAssetProcessor);

	packageWalker.LoadAllAssets(binaryReader);

	m_pSpriteAssetProcessor->FixupAllAssets(*m_pImageAssetContainer);

	m_pFigureAsset = m_pSpriteAssetContainer->GetFigureIterator().GetValue();
}

void SpriteViewerMode::BuildBindings()
{

}

void SpriteViewerMode::BuildInstances()
{
	m_pFigureInstance = m_pInstanceBuilder->CreateFigureInstance(*m_pFigureAsset);

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
	m_pAnimationAsset = m_pSpriteAssetContainer->FindAnimationAsset(pName);

	f32 frameMax = 0.0f;
	if (m_pAnimationAsset != NULL)
	{
		// create the binding
		m_pBindingBuilder->SetAllocator(&m_animationAllocator);
		m_pAnimationFigureBinding = m_pBindingBuilder->CreateAnimationFigureBinding(*m_pAnimationAsset, *m_pFigureAsset);

		// create the instance and bind to the figure
		m_pInstanceBuilder->SetAllocator(&m_animationAllocator);
		m_pAnimationInstance = m_pInstanceBuilder->CreateAnimationInstance(m_pAnimationAsset->pAnimationData->trackCount);
		m_pAnimationInstance->BindFigureInstance(*m_pFigureInstance);
		m_pAnimationInstance->BindAnimationAsset(*m_pAnimationAsset, *m_pAnimationFigureBinding);

		frameMax = m_pAnimationInstance->FindLongestTrackFrameDuration();
	}

	// update dependent menu items
	m_pFrameMenuItem->SetMax(frameMax);
	m_pFrameMenuItem->SetValue(0.0f);
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
	f32 stepValue = m_pStepMenuItem->GetValue();
	m_pFrameMenuItem->SetStep(stepValue);

	return 0;
}

int SpriteViewerMode::OnFrameCycle(float parameter)
{
	PlayType playType = static_cast<PlayType>(m_pPlayMenuItem->GetIndex());
	if (playType == PLAY_TYPE_MANUAL)
	{
		f32 frameValue = m_pFrameMenuItem->GetValue();
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
