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
static const f32 MENU_ITEM_MIDDLE_OFFSET_X = 280.0f;
static const f32 MENU_ITEM_RIGHT_OFFSET_X = 400.0f;

/*
* Public Instance
*/
SoundViewerMode::SoundViewerMode()
: AssetViewerMode()

, m_assetContainer()
, m_soundAssetHandler()
, m_pAssetBinary(NULL)

//, m_waveResource()
, m_clipCoordinator()

, m_menu()
, m_menuPage()
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

	m_clipCoordinator.CreateStructure(20, m_shared.pAllocator);

	LoadAssets();
	BuildMenu();;

	m_clearColor.r = 0.5f;
	m_clearColor.g = 0.5f;
	m_clearColor.b = 0.5f;
	m_clearColor.a = 1.0f;

	// TEMP!
	/*const f32 FREQ = 440.f;
	const u32 AMPLITUIDE = 32760;
	const u32 SECONDS = 4;
	const u32 SAMPLE_RATE = 22050;
	const u32 BUF_SIZE = SECONDS * SAMPLE_RATE;

	u16 samples[BUF_SIZE];
	for (int i = 0; i < BUF_SIZE; ++i)
	{
		samples[i] = AMPLITUIDE * lct::foun::Sin((2.0f * lct::foun::PI_32 * FREQ) / SAMPLE_RATE * i);
	}*/

	/*
	lct::audi::WaveSetupParameters waveParams;
	waveParams.pWaveResource = &m_waveResource;
	waveParams.pWaveBinary = samples;
	waveParams.size = BUF_SIZE * sizeof(u16);
	waveParams.channelCount = 1;
	waveParams.sampleSize = 2;
	waveParams.sampleRate = SAMPLE_RATE;
	m_shared.pAudioDevice->AcquireWaveResource(waveParams);
	*/

	//lct::audi::VoiceResource* pVoice = m_shared.pAudioDevice->UseVoice();
	//lct::soun::WaveAsset* pWaveAsset = m_assetContainer.FindAsset<lct::soun::WaveAsset>("horn_mono_16");
	//m_shared.pAudioDevice->PlayVoice(pVoice, pWaveAsset->pWaveResource);
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
	m_clipCoordinator.AcquireAudioResources(m_shared.pAudioDevice);

	AssetViewerMode::AcquireAudio();
}

void SoundViewerMode::ReleaseAudio()
{
	m_soundAssetHandler.ReleaseAllAssetResources();

	// TEMP!
	m_clipCoordinator.ReleaseAudioResources(m_shared.pAudioDevice);

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

	// TEMP!	
	m_clipCoordinator.ApplyClips(m_shared.pAudioDevice);
	m_clipCoordinator.UpdateRamps(1.0f / 60.0f);
}

void SoundViewerMode::Draw()
{
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

	m_menu.SetSpacing(pSheetAsset->pSheetData->lineHeight * 2.0f);
	lct::foun::Vector2 menuPosition = { MENU_OFFSET_X, MENU_OFFSET_Y };
	m_menu.SetPosition(menuPosition);

	m_menuPage.SetLabel("Default");
	{
		m_playMenuItem.SetLabel("Play");
		m_playMenuItem.SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X);
		m_playMenuItem.GetCallback().Bind(this, &SoundViewerMode::OnPlayTrigger);
		m_menuPage.AddItem(&m_playMenuItem);
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

void SoundViewerMode::OnPlayTrigger()
{
	lct::soun::ClipAsset* pClipAsset = m_assetContainer.FindAsset<lct::soun::ClipAsset>("example_clip_tone_A_4");
	lct::soun::RampAsset* pRampAsset = m_assetContainer.FindAsset<lct::soun::RampAsset>("example_ramp_note");
	m_clipCoordinator.StartClip(pClipAsset, pRampAsset);
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
