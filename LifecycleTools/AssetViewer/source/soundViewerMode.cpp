#include "soundViewerMode.h"
#include "assetViewerMessages.h"

#include <foun/foun_math.h>
#include <foun/foun_debug.h>

#include <string.h>

/*
* Internal Constants
*/
static const u32 SYMBOL_BUFFER_QUAD_COUNT = 512;

static const f32 MENU_OFFSET_X = 0.0f;
static const f32 MENU_OFFSET_Y = -100.0f;

static const f32 MENU_ITEM_LEFT_OFFSET_X = 160.0f;
static const f32 MENU_ITEM_MIDDLE_OFFSET_X = 300.0f;
static const f32 MENU_ITEM_RIGHT_OFFSET_X = 440.0f;

static const f32 BAR_WIDTH = 128.0f;
static const f32 BAR_HEIGHT = 8.0f;
static const f32 BAR_SPACING = 8.0f;
static const lct::foun::FloatColor VOLUME_BAR_COLOR = { 0.0f, 0.0f, 1.0f, 1.0f };
static const lct::foun::FloatColor PITCH_BAR_COLOR = { 1.0f, 1.0f, 0.0f, 1.0f };

static const f32 CONTENT_OFFSET_X = 200.0f;
static const f32 CONTENT_HEIGHT = SoundViewerMode::CLIP_CAPACITY * ((BAR_HEIGHT * 2.0f) + BAR_SPACING);

/*
* Public Instance
*/
SoundViewerMode::SoundViewerMode()
: AssetViewerMode()

, m_assetContainer()
, m_soundAssetHandler()
, m_pAssetBinary(NULL)

// TEMP!
, m_clipCoordinator()
, m_sequenceCoordinator()

, m_clipHandle()
, m_sequenceHandle()

, m_clipStatsArray()

, m_pClipNameArray(NULL)
, m_pRampNameArray(NULL)

, m_menu()
, m_menuPage()
, m_clipMenuItem()
, m_rampMenuItem()
, m_beginClipMenuItem()
, m_endClipMenuItem()
, m_sequenceMenuItem()
, m_beginSequenceMenuItem()
, m_endSequenceMenuItem()
, m_reloadMenuItem()

, m_projectionTransform()
, m_contentWorldTransform()
{
}

SoundViewerMode::~SoundViewerMode()
{
}

void SoundViewerMode::Init()
{
	AssetViewerMode::Init();

	m_assetContainer.SetAllocator(m_shared.pAllocator);

	m_soundAssetHandler.SetAllocator(m_shared.pAllocator);
	m_soundAssetHandler.SetAudioDevice(m_shared.pAudioDevice);
	m_soundAssetHandler.SetAssetContainer(&m_assetContainer);

	m_clipCoordinator.SetAudioDevice(m_shared.pAudioDevice);
	m_clipCoordinator.CreateStructure(CLIP_CAPACITY, m_shared.pAllocator);

	m_sequenceCoordinator.SetClipCoordinator(&m_clipCoordinator);
	m_sequenceCoordinator.CreateStructure(SEQUENCE_CAPACITY, m_shared.pAllocator);

	LoadAssets();
	BuildMenu();

	m_clearColor.r = 0.5f;
	m_clearColor.g = 0.5f;
	m_clearColor.b = 0.5f;
	m_clearColor.a = 1.0f;
}

void SoundViewerMode::AcquireGraphics()
{
	m_menu.AcquireGraphics();

	AssetViewerMode::AcquireGraphics();
}

void SoundViewerMode::ReleaseGraphics()
{
	m_menu.ReleaseGraphics();

	AssetViewerMode::ReleaseGraphics();
}

void SoundViewerMode::AcquireAudio()
{
	m_soundAssetHandler.AcquireAllAssetResources();

	// TEMP!
	m_clipCoordinator.AcquireAudioResources();

	AssetViewerMode::AcquireAudio();
}

void SoundViewerMode::ReleaseAudio()
{
	m_soundAssetHandler.ReleaseAllAssetResources();

	// TEMP!
	m_clipCoordinator.ReleaseAudioResources();

	AssetViewerMode::ReleaseAudio();
}

void SoundViewerMode::ReadInput()
{
	m_menu.HandleInput();
}

void SoundViewerMode::Update()
{
	const lct::foun::RectEdges& screenEdges = m_shared.pScreen->GetRectEdges();
	lct::foun::Matrix44OrthographicProjection(m_projectionTransform, screenEdges.left, screenEdges.right, screenEdges.bottom, screenEdges.top, -1.0f, 1.0f);

	lct::foun::Matrix32Translate(m_contentWorldTransform, CONTENT_OFFSET_X, 0.0f);

	// TEMP!
	f32 secondStep = 1.0f / 60.0f;
	m_clipCoordinator.Update(secondStep);
	m_sequenceCoordinator.Update(secondStep);

	m_clipCoordinator.FillClipStats(m_clipStatsArray, CLIP_CAPACITY);
}

void SoundViewerMode::Draw()
{
	m_subShared.pFillDrawContext->ActivateRenderState();
	m_subShared.pFillDrawContext->ActivateShader();
	m_subShared.pFillDrawContext->ActivateQuad();
	m_subShared.pFillDrawContext->ActivateProjectionTransform(m_projectionTransform);
	m_subShared.pFillDrawContext->ActivateWorldTransform(m_contentWorldTransform);

	f32 clipBarY = CONTENT_HEIGHT / 2.0f;
	for (u32 clipIndex = 0; clipIndex < CLIP_CAPACITY; ++clipIndex)
	{
		lct::soun::ClipStats& clipStats = m_clipStatsArray[clipIndex];

		lct::foun::RectEdges volumeRect;
		volumeRect.left = 0.0f;
		volumeRect.right = BAR_WIDTH * clipStats.volume;
		volumeRect.bottom = clipBarY - BAR_HEIGHT;
		volumeRect.top = clipBarY;
		clipBarY -= BAR_HEIGHT;

		lct::foun::RectEdges pitchRect;
		pitchRect.left = 0.0f;
		pitchRect.right = BAR_WIDTH * clipStats.pitch;
		pitchRect.bottom = clipBarY - BAR_HEIGHT;
		pitchRect.top = clipBarY;
		clipBarY -= BAR_HEIGHT;

		if (clipStats.playing)
		{
			m_subShared.pFillDrawContext->DrawRect(volumeRect, VOLUME_BAR_COLOR);
			m_subShared.pFillDrawContext->DrawRect(pitchRect, PITCH_BAR_COLOR);
		}

		clipBarY -= BAR_SPACING;
	}	

	m_subShared.pFillDrawContext->DeactivateQuad();

	m_menu.Draw();
}

/*
* Private Instance
*/
void SoundViewerMode::BuildMenu()
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

	u32 clipNameCount = m_assetContainer.GetAssetCount<lct::soun::ClipAsset>() + 1;
	m_pClipNameArray = m_shared.pAllocator->AllocTypeArray<const char*>(clipNameCount);
	m_pClipNameArray[0] = "<NONE>";
	lct::pack::AssetIterator<lct::soun::ClipAsset> clipIterator = m_assetContainer.GetIterator<lct::soun::ClipAsset>();
	for (u32 clipNameIndex = 1; clipNameIndex < clipNameCount; ++clipNameIndex)
	{
		m_pClipNameArray[clipNameIndex] = clipIterator.GetAsset()->pClipData->name;
		clipIterator.Next();
	}

	u32 rampNameCount = m_assetContainer.GetAssetCount<lct::soun::RampAsset>() + 1;
	m_pRampNameArray = m_shared.pAllocator->AllocTypeArray<const char*>(rampNameCount);
	m_pRampNameArray[0] = "<NONE>";
	lct::pack::AssetIterator<lct::soun::RampAsset> rampIterator = m_assetContainer.GetIterator<lct::soun::RampAsset>();
	for (u32 rampNameIndex = 1; rampNameIndex < rampNameCount; ++rampNameIndex)
	{
		m_pRampNameArray[rampNameIndex] = rampIterator.GetAsset()->pRampData->name;
		rampIterator.Next();
	}

	u32 sequenceNameCount = m_assetContainer.GetAssetCount<lct::soun::SequenceAsset>() + 1;
	m_pSequenceNameArray = m_shared.pAllocator->AllocTypeArray<const char*>(sequenceNameCount);
	m_pSequenceNameArray[0] = "<NONE>";
	lct::pack::AssetIterator<lct::soun::SequenceAsset> sequenceIterator = m_assetContainer.GetIterator<lct::soun::SequenceAsset>();
	for (u32 sequenceNameIndex = 1; sequenceNameIndex < sequenceNameCount; ++sequenceNameIndex)
	{
		m_pSequenceNameArray[sequenceNameIndex] = sequenceIterator.GetAsset()->pSequenceData->name;
		sequenceIterator.Next();
	}

	m_menu.SetSpacing(pSheetAsset->pSheetData->lineHeight * 2.0f);
	lct::foun::Vector2 menuPosition = { MENU_OFFSET_X, MENU_OFFSET_Y };
	m_menu.SetPosition(menuPosition);

	m_menuPage.SetLabel("Default");
	{
		m_clipMenuItem.SetLabel("Clip");
		m_clipMenuItem.SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X, MENU_ITEM_LEFT_OFFSET_X, MENU_ITEM_RIGHT_OFFSET_X);
		m_clipMenuItem.SetStringArray(m_pClipNameArray);
		m_clipMenuItem.SetCount(clipNameCount);
		m_clipMenuItem.SetIndex(0);
		//m_clipMenuItem.GetCycleCallback().Bind(this, &SpriteViewerMode::OnClipCycle);
		m_menuPage.AddItem(&m_clipMenuItem);
	}
	{
		m_rampMenuItem.SetLabel("Ramp");
		m_rampMenuItem.SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X, MENU_ITEM_LEFT_OFFSET_X, MENU_ITEM_RIGHT_OFFSET_X);
		m_rampMenuItem.SetStringArray(m_pRampNameArray);
		m_rampMenuItem.SetCount(rampNameCount);
		m_rampMenuItem.SetIndex(0);
		//m_rampMenuItem.GetCycleCallback().Bind(this, &SpriteViewerMode::OnRampCycle);
		m_menuPage.AddItem(&m_rampMenuItem);
	}
	{
		m_beginClipMenuItem.SetLabel("Begin Clip");
		m_beginClipMenuItem.SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X);
		m_beginClipMenuItem.GetCallback().Bind(this, &SoundViewerMode::OnBeginClipTrigger);
		m_menuPage.AddItem(&m_beginClipMenuItem);
	}
	{
		m_endClipMenuItem.SetLabel("End Clip");
		m_endClipMenuItem.SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X);
		m_endClipMenuItem.GetCallback().Bind(this, &SoundViewerMode::OnEndClipTrigger);
		m_menuPage.AddItem(&m_endClipMenuItem);
	}
	{
		m_sequenceMenuItem.SetLabel("Sequence");
		m_sequenceMenuItem.SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X, MENU_ITEM_LEFT_OFFSET_X, MENU_ITEM_RIGHT_OFFSET_X);
		m_sequenceMenuItem.SetStringArray(m_pSequenceNameArray);
		m_sequenceMenuItem.SetCount(sequenceNameCount);
		m_sequenceMenuItem.SetIndex(0);
		m_menuPage.AddItem(&m_sequenceMenuItem);
	}
	{
		m_beginSequenceMenuItem.SetLabel("Begin Sequence");
		m_beginSequenceMenuItem.SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X);
		m_beginSequenceMenuItem.GetCallback().Bind(this, &SoundViewerMode::OnBeginSequenceTrigger);
		m_menuPage.AddItem(&m_beginSequenceMenuItem);
	}
	{
		m_endSequenceMenuItem.SetLabel("End Sequence");
		m_endSequenceMenuItem.SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X);
		m_endSequenceMenuItem.GetCallback().Bind(this, &SoundViewerMode::OnEndSequenceTrigger);
		m_menuPage.AddItem(&m_endSequenceMenuItem);
	}
	{
		m_reloadMenuItem.SetLabel("Reload");
		m_reloadMenuItem.SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X);
		m_reloadMenuItem.GetCallback().Bind(this, &SoundViewerMode::OnReloadTrigger);
		m_menuPage.AddItem(&m_reloadMenuItem);
	}
	m_menu.AddPage(&m_menuPage);
	m_menu.Arrange();
	m_menu.ActivatePage("Default");
}

void SoundViewerMode::LoadAssets()
{
	static const char* FILE_PATH = "data/soundViewer/package.bin";
	u32 fileSize;
	m_pAssetBinary = m_subShared.pAccessor->LoadFile(FILE_PATH, &fileSize);
	lct::util::BinaryReader binaryReader;
	binaryReader.SetMemory(m_pAssetBinary, fileSize);
	lct::pack::PackageWalker packageWalker;
	packageWalker.AddAssetHandler(&m_soundAssetHandler);

	packageWalker.LoadAllAssets(binaryReader);
	
	m_soundAssetHandler.FixupAllAssets();
}

void SoundViewerMode::OnBeginClipTrigger()
{
	const char* pClipName = m_clipMenuItem.GetValue();
	lct::soun::ClipAsset* pClipAsset = m_assetContainer.FindAsset<lct::soun::ClipAsset>(pClipName);
	const char* pRampName = m_rampMenuItem.GetValue();
	lct::soun::RampAsset* pRampAsset = m_assetContainer.FindAsset<lct::soun::RampAsset>(pRampName);
	if (pClipAsset != NULL)
	{
		m_clipCoordinator.BeginClip(&m_clipHandle, pClipAsset);
		if (pRampAsset != NULL)
		{
			m_clipCoordinator.SetRamp(&m_clipHandle, pRampAsset);
		}
	}
}

void SoundViewerMode::OnEndClipTrigger()
{
	const char* pRampName = m_rampMenuItem.GetValue();
	lct::soun::RampAsset* pRampAsset = m_assetContainer.FindAsset<lct::soun::RampAsset>(pRampName);
	if (pRampAsset != NULL)
	{
		m_clipCoordinator.SetRamp(&m_clipHandle, pRampAsset);
	}
	else
	{
		m_clipCoordinator.ClearRamp(&m_clipHandle);
	}
	m_clipCoordinator.EndClip(&m_clipHandle);
}

void SoundViewerMode::OnBeginSequenceTrigger()
{
	const char* pSequenceName = m_sequenceMenuItem.GetValue();
	lct::soun::SequenceAsset* pSequenceAsset = m_assetContainer.FindAsset<lct::soun::SequenceAsset>(pSequenceName);
	if (pSequenceAsset != NULL)
	{
		m_sequenceCoordinator.BeginSequence(&m_sequenceHandle, pSequenceAsset);
	}
}

void SoundViewerMode::OnEndSequenceTrigger()
{
	m_sequenceCoordinator.EndSequence(&m_sequenceHandle);
}

void SoundViewerMode::OnReloadTrigger()
{
	lct::fram::Message message;
	message.SetType(MESSAGE_TYPE_PROGRAM_CHANGE_MODE);
	ProgramChangeModeContent content;
	content.pNextModeName = "SoundViewerMode";
	message.SetContent(&content);
	m_shared.pProgramMessageQueue->EnqueueMessage(message);
}
