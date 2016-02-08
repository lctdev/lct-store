#include "particleViewerMode.h"
#include "assetViewerMessages.h"
#include "assetViewerConstants.h"

#include <foun/foun_math.h>
#include <foun/foun_string.h>
#include <foun/foun_debug.h>

#include <string.h>

/*
 * Internal Constants
 */
static const f32 MENU_OFFSET_X = 0.0f;
static const f32 MENU_OFFSET_Y = -100.0f;

static const f32 MENU_ITEM_LEFT_OFFSET_X = 160.0f;
static const f32 MENU_ITEM_MIDDLE_OFFSET_X = 280.0f;
static const f32 MENU_ITEM_RIGHT_OFFSET_X = 400.0f;

static const f32 AXIS_LINE_LENGTH = 512.0f;
static const f32 AXIS_LINE_THICKNESS = 1.0f;
static const lct::foun::FloatColor4 AXIS_LINE_COLOR = { 0.25f, 0.25f, 0.25f, 1.0f };

//static const lct::foun::RectCentered JOINT_RECT = { 0.0f, 0.0f, 4.0f, 4.0f };
//static const lct::foun::FloatColor4 JOINT_COLOR = { 0.0f, 1.0f, 0.0f, 1.0f };

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

static const u32 EMITTER_CAPACITY = 10;
static const u32 PARTICLE_CAPACITY = 1000;

static const f32 MAX_CYCLE_FRAME = 300.0f;

/*
 * Public Instance
 */
ParticleViewerMode::ParticleViewerMode()
: AssetViewerMode()

, m_assetContainer()
//, m_spriteAssetHandler()
//, m_imageAssetHandler()
//, m_pAssetBinary(NULL)
, m_pFieldAsset(NULL)
, m_pFieldInstance()

, m_pFieldNameArray(NULL)

, m_menu()
, m_menuPage()
, m_fieldMenuItem()
, m_playMenuItem()
, m_stepMenuItem()
, m_frameMenuItem()
//, m_drawJointsMenuItem()
, m_reloadMenuItem()

, m_projectionTransform()
, m_contentWorldTransform()
{
}

ParticleViewerMode::~ParticleViewerMode()
{
}

void ParticleViewerMode::Init()
{
	AssetViewerMode::Init();

	m_assetContainer.SetAllocator(m_shared.pAllocator);

	//m_spriteAssetHandler.SetAllocator(m_shared.pAllocator);
	//m_spriteAssetHandler.SetAssetContainer(&m_assetContainer);

	//m_imageAssetHandler.SetAllocator(m_shared.pAllocator);
	//m_imageAssetHandler.SetGraphicsDevice(m_shared.pGraphicsDevice);
	//m_imageAssetHandler.SetAssetContainer(&m_assetContainer);

	LoadAssets();
	BuildMenu();
	BuildInstances();

	const char* pFieldName = m_pFieldNameArray[0];
	BindField(pFieldName);

	m_clearColor.r = 0.5f;
	m_clearColor.g = 0.5f;
	m_clearColor.b = 0.5f;
	m_clearColor.a = 1.0f;
}

void ParticleViewerMode::AcquireGraphics()
{
	//m_imageAssetHandler.AcquireAllAssetResources();

	m_menu.AcquireGraphics();
	
	m_pFieldInstance->AcquireResources(m_shared.pGraphicsDevice);

	AssetViewerMode::AcquireGraphics();
}

void ParticleViewerMode::ReleaseGraphics()
{
	//m_imageAssetHandler.ReleaseAllAssetResources();

	m_menu.ReleaseGraphics();
	
	m_pFieldInstance->ReleaseResources(m_shared.pGraphicsDevice);

	AssetViewerMode::ReleaseGraphics();
}

void ParticleViewerMode::ReadInput()
{
	m_menu.HandleInput();
}

void ParticleViewerMode::Update()
{
	const lct::foun::RectEdges& screenEdges = m_shared.pScreen->GetRectEdges();
	lct::foun::Matrix44OrthographicProjection(m_projectionTransform, screenEdges.left, screenEdges.right, screenEdges.bottom, screenEdges.top, -1.0f, 1.0f);

	lct::foun::Matrix32Translate(m_contentWorldTransform, CONTENT_OFFSET_X, 0.0f);

	PlayType playType = static_cast<PlayType>(m_playMenuItem.GetIndex());
	if (playType == PLAY_TYPE_AUTOMATIC)
	{
		if (m_pFieldAsset != NULL)
		{
			m_pFieldInstance->UpdateEmitters(m_stepMenuItem.GetValue());
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
			if (frameValue > frameMax)
			{
				frameValue = frameMax;
			}
		}
		m_frameMenuItem.SetValue(frameValue);
	}
}

void ParticleViewerMode::Draw()
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

	// particle
	if (m_pFieldAsset != NULL)
	{
		m_pFieldInstance->UpdateParticles();
		m_pFieldInstance->RefreshResources(m_shared.pGraphicsDevice);

		m_subShared.pParticleDrawContext->ActivateRenderState();
		m_subShared.pParticleDrawContext->ActivateShader();
		m_subShared.pParticleDrawContext->ActivateFieldInstance(*m_pFieldInstance);
		m_subShared.pParticleDrawContext->ActivateProjectionTransform(m_projectionTransform);

		m_subShared.pParticleDrawContext->ActivateWorldTransform(m_contentWorldTransform);
		m_subShared.pParticleDrawContext->DrawField(*m_pFieldInstance);

		m_subShared.pParticleDrawContext->DeactivateFieldInstance(*m_pFieldInstance);
		
		m_pFieldInstance->SwapVertexResource();
	}

/*
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
*/

	// menu
	m_menu.Draw();
}

/*
 * Private Instance
 */
void ParticleViewerMode::BuildMenu()
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

	u32 fieldNameCount = m_assetContainer.GetAssetCount<lct::part::FieldAsset>() + 1;
	m_pFieldNameArray = m_shared.pAllocator->AllocTypeArray<const char*>(fieldNameCount);
	m_pFieldNameArray[0] = "<NONE>";
	lct::pack::AssetIterator<lct::part::FieldAsset> fieldIterator = m_assetContainer.GetIterator<lct::part::FieldAsset>();
	for (u32 fieldNameIndex = 1; fieldNameIndex < fieldNameCount; ++fieldNameIndex)
	{
		m_pFieldNameArray[fieldNameIndex] = fieldIterator.GetAsset()->pFieldData->name;
		fieldIterator.Next();
	}

	m_menu.SetSpacing(pSheetAsset->pSheetData->lineHeight * 2.0f);
	lct::foun::Vector2 menuPosition = { MENU_OFFSET_X, MENU_OFFSET_Y };
	m_menu.SetPosition(menuPosition);
	m_menu.SetPressSize(PRESS_SIZE);

	m_menuPage.SetLabel("Default");
	{
		m_fieldMenuItem.SetLabel("Field");
		m_fieldMenuItem.SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X, MENU_ITEM_LEFT_OFFSET_X, MENU_ITEM_RIGHT_OFFSET_X);
		m_fieldMenuItem.SetStringArray(m_pFieldNameArray);
		m_fieldMenuItem.SetCount(fieldNameCount);
		m_fieldMenuItem.SetIndex(0);
		m_fieldMenuItem.GetCycleCallback().Bind(this, &ParticleViewerMode::OnFieldCycle);
		m_menuPage.AddItem(&m_fieldMenuItem);
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
		m_stepMenuItem.GetCycleCallback().Bind(this, &ParticleViewerMode::OnStepCycle);
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
		m_frameMenuItem.GetCycleCallback().Bind(this, &ParticleViewerMode::OnFrameCycle);
		m_menuPage.AddItem(&m_frameMenuItem);
	}
	/*
	{
		m_drawJointsMenuItem.SetLabel("Draw Joints");
		m_drawJointsMenuItem.SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X, MENU_ITEM_LEFT_OFFSET_X, MENU_ITEM_RIGHT_OFFSET_X);
		m_drawJointsMenuItem.SetValue(false);
		m_menuPage.AddItem(&m_drawJointsMenuItem);
	}
	*/
	{
		m_reloadMenuItem.SetLabel("Reload");
		m_reloadMenuItem.SetOffsets(MENU_ITEM_MIDDLE_OFFSET_X);
		m_reloadMenuItem.GetCallback().Bind(this, &ParticleViewerMode::OnReloadTrigger);
		m_menuPage.AddItem(&m_reloadMenuItem);
	}
	m_menu.AddPage(&m_menuPage);
	m_menu.Arrange();
	m_menu.ActivatePage("Default");
}

void ParticleViewerMode::LoadAssets()
{
/*
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
	*/
	
	{
		static const char* FIELD_NAME = "Test1";
		static const u32 EMITTER_COUNT = 1;
		lct::part::EmitterData* pEmitterDatas = m_shared.pAllocator->AllocTypeArray<lct::part::EmitterData>(EMITTER_COUNT);
		lct::part::FieldAsset::Emitter* pAssetEmitters = m_shared.pAllocator->AllocTypeArray<lct::part::FieldAsset::Emitter>(EMITTER_COUNT);
		{
			static const u32 EMITTER_INDEX = 0;
			lct::part::EmitterData* pEmitterData = pEmitterDatas + EMITTER_INDEX;
			pEmitterData->x = 0.0f;
			pEmitterData->y = 0.0f;			
			pEmitterData->shapeSpanA = 0.0f;
			pEmitterData->shapeSpanB = 0.0f;
			pEmitterData->globalDirX = 0.0f;
			pEmitterData->globalDirY = -1.0f;
			pEmitterData->expelAngleRange.min = 0.20f;
			pEmitterData->expelAngleRange.max = 0.30f;
			pEmitterData->color0.r = 1.0f;
			pEmitterData->color0.g = 1.0f;
			pEmitterData->color0.b = 1.0f;
			pEmitterData->color1.r = 0.0f;
			pEmitterData->color1.g = 0.0f;
			pEmitterData->color1.b = 1.0f;
			pEmitterData->delayFrames = 4.0f;
			pEmitterData->particleCount = 32;
			pEmitterData->shapeType = lct::part::EMITTER_SHAPE_TYPE_POINT;
			pEmitterData->arrangeType = lct::part::EMITTER_ARRANGE_TYPE_EVEN_EDGE;
			pEmitterData->expelType = lct::part::EMITTER_EXPEL_TYPE_DEFAULT;
			pEmitterData->flags = (1 << lct::part::EMITTER_FLAG_TYPE_REVERSE_ORDER);
		
			{
				lct::part::ParticleParameterData& parameterData = pEmitterData->aParticleParameterData[lct::part::PARTICLE_PROPERTY_TYPE_GLOBAL_DISTANCE];
				parameterData.initial = 0.0f;
				parameterData.velocity = 0.0f;
				parameterData.acceleration = 0.2f;
				parameterData.frameRange.min = 0.0f;
				parameterData.frameRange.max = 600.0f;

				lct::part::Range& multiplierRange = pEmitterData->aParticleMultiplierRanges[lct::part::PARTICLE_PROPERTY_TYPE_GLOBAL_DISTANCE];
				multiplierRange.min = 1.0f;
				multiplierRange.max = 1.0f;
			}
			{
				lct::part::ParticleParameterData& parameterData = pEmitterData->aParticleParameterData[lct::part::PARTICLE_PROPERTY_TYPE_EXPEL_DISTANCE];
				parameterData.initial = 0.0f;
				parameterData.velocity = 6.0f;
				parameterData.acceleration = 0.0f;
				parameterData.frameRange.min = 0.0f;
				parameterData.frameRange.max = 600.0f;

				lct::part::Range& multiplierRange = pEmitterData->aParticleMultiplierRanges[lct::part::PARTICLE_PROPERTY_TYPE_EXPEL_DISTANCE];
				multiplierRange.min = 1.0f;
				multiplierRange.max = 1.5f;
			}
			{
				lct::part::ParticleParameterData& parameterData = pEmitterData->aParticleParameterData[lct::part::PARTICLE_PROPERTY_TYPE_SIZE];
				parameterData.initial = 16.0f;
				parameterData.velocity = 0.0f;
				parameterData.acceleration = 0.0f;
				parameterData.frameRange.min = 0.0f;
				parameterData.frameRange.max = 600.0f;

				lct::part::Range& multiplierRange = pEmitterData->aParticleMultiplierRanges[lct::part::PARTICLE_PROPERTY_TYPE_SIZE];
				multiplierRange.min = 0.8f;
				multiplierRange.max = 1.2f;
			}
			{
				lct::part::ParticleParameterData& parameterData = pEmitterData->aParticleParameterData[lct::part::PARTICLE_PROPERTY_TYPE_ROTATION];
				parameterData.initial = 0.0f;
				parameterData.velocity = 0.01f;
				parameterData.acceleration = 0.0f;
				parameterData.frameRange.min = 0.0f;
				parameterData.frameRange.max = 600.0f;


				lct::part::Range& multiplierRange = pEmitterData->aParticleMultiplierRanges[lct::part::PARTICLE_PROPERTY_TYPE_ROTATION];
				multiplierRange.min = 0.8f;
				multiplierRange.max = 1.2f;
			}
			{
				lct::part::ParticleParameterData& parameterData = pEmitterData->aParticleParameterData[lct::part::PARTICLE_PROPERTY_TYPE_COLOR_RATIO];
				parameterData.initial = 0.0f;
				parameterData.velocity = 0.01f;
				parameterData.acceleration = 0.0f;
				parameterData.frameRange.min = 0.0f;
				parameterData.frameRange.max = 600.0f;

				lct::part::Range& multiplierRange = pEmitterData->aParticleMultiplierRanges[lct::part::PARTICLE_PROPERTY_TYPE_COLOR_RATIO];
				multiplierRange.min = 1.0f;
				multiplierRange.max = 1.0f;
			}
			{
				lct::part::ParticleParameterData& parameterData = pEmitterData->aParticleParameterData[lct::part::PARTICLE_PROPERTY_TYPE_ALPHA];
				parameterData.initial = 1.0f;
				parameterData.velocity = -1.0f / 15.0f;
				parameterData.acceleration = 0.0f;
				parameterData.frameRange.min = 75.0f;
				parameterData.frameRange.max = 90.0f;

				lct::part::Range& multiplierRange = pEmitterData->aParticleMultiplierRanges[lct::part::PARTICLE_PROPERTY_TYPE_ALPHA];
				multiplierRange.min = 1.0f;
				multiplierRange.max = 1.0f;
			}
			
			lct::part::FieldAsset::Emitter* pAssetEmitter = pAssetEmitters + EMITTER_INDEX;
			pAssetEmitter->pEmitterData = pEmitterData;
		}
		
		lct::part::FieldData* pFieldData = m_shared.pAllocator->AllocType<lct::part::FieldData>();
		LCT_STRCPY(pFieldData->name, sizeof(pFieldData->name), FIELD_NAME);
		pFieldData->emitterCount = EMITTER_COUNT;
		
		lct::part::FieldAsset* pFieldAsset = m_shared.pAllocator->AllocType<lct::part::FieldAsset>();
		pFieldAsset->pFieldData = pFieldData;
		pFieldAsset->pEmitters = pAssetEmitters;
		
		m_assetContainer.AddAsset(pFieldAsset, pFieldData->name);
	}
}

void ParticleViewerMode::BuildInstances()
{
	m_pFieldInstance = m_shared.pAllocator->AllocType<lct::part::FieldInstance>();
	m_pFieldInstance->CreateStructure(EMITTER_CAPACITY, PARTICLE_CAPACITY, m_shared.pAllocator);
}

void ParticleViewerMode::BindField(const char* pName)
{
	// find the specified asset
	m_pFieldAsset = m_assetContainer.FindAsset<lct::part::FieldAsset>(pName);

	f32 frameMax = 0.0f;
	if (m_pFieldAsset != NULL)
	{
		// bind to the field
		m_pFieldInstance->BindFieldAsset(m_pFieldAsset);
		m_pFieldInstance->SetRandomSeed(0);

		frameMax = MAX_CYCLE_FRAME; // TEMP! Check against emitters?
	}

	// update dependent menu items
	m_frameMenuItem.SetMax(frameMax);
	m_frameMenuItem.SetValue(0.0f);
}

void ParticleViewerMode::OnFieldCycle()
{
	u32 fieldIndex = m_fieldMenuItem.GetIndex();
	const char* pFieldName = m_pFieldNameArray[fieldIndex];
	BindField(pFieldName);
}

void ParticleViewerMode::OnStepCycle()
{
	// update dependent menu items
	f32 stepValue = m_stepMenuItem.GetValue();
	m_frameMenuItem.SetStep(stepValue);
}

void ParticleViewerMode::OnFrameCycle()
{
	PlayType playType = static_cast<PlayType>(m_playMenuItem.GetIndex());
	if (playType == PLAY_TYPE_MANUAL)
	{
		f32 frameValue = m_frameMenuItem.GetValue();
		m_pFieldInstance->SetRandomSeed(0);
		m_pFieldInstance->ForceFrameOnEmitters(frameValue);
	}
}

void ParticleViewerMode::OnReloadTrigger()
{
	lct::fram::Message message;
	message.SetType(MESSAGE_TYPE_PROGRAM_CHANGE_MODE);
	ProgramChangeModeContent content;
	content.pNextModeName = "ParticleViewerMode";
	message.SetContent(&content);
	m_shared.pProgramMessageQueue->EnqueueMessage(message);
}

