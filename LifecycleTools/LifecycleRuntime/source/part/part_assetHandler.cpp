#include <part/part_assetHandler.h>
#include <part/part_assets.h>
#include <part/part_data.h>
#include <part/part_constants.h>

#include <imag/imag_assets.h>
#include <imag/imag_data.h>

#include <pack/pack_assetContainer.h>
#include <pack/pack_assetHeader.h>

#include <util/util_binaryReader.h>
#include <util/util_indexMap.h>

#include <foun/foun_math.h>

namespace lct
{
namespace part
{

/*
* Internal Constants
*/

/*
* Public Instance
*/
AssetHandler::AssetHandler()
	: m_pAllocator(NULL)
	, m_pAssetContainer(NULL)
{
}

AssetHandler::~AssetHandler()
{
}

void AssetHandler::SetAllocator(lct::foun::Allocator* pAllocator)
{
	m_pAllocator = pAllocator;
}

void AssetHandler::SetAssetContainer(pack::AssetContainer* pAssetContainer)
{
	m_pAssetContainer = pAssetContainer;
}

bool AssetHandler::LoadAsset(pack::AssetHeader& assetHeader, util::BinaryReader& binaryReader)
{
	bool assetLoaded = false;
	if ((assetHeader.groupCode.numeric == GROUP_CODE.numeric) && (assetHeader.version == VERSION))
	{
		if (assetHeader.typeCode.numeric == FIELD_TYPE_CODE.numeric)
		{
			FieldAsset* pFieldAsset = LoadFieldAsset(binaryReader);
			m_pAssetContainer->AddAsset(pFieldAsset, pFieldAsset->pFieldData->name);
			assetLoaded = true;
		}
	}
	return assetLoaded;
}

FieldAsset* AssetHandler::LoadFieldAsset(util::BinaryReader& binaryReader)
{
	FieldData* pFieldData = binaryReader.ReadType<FieldData>();

	FieldAsset::Emitter* paEmitters = m_pAllocator->AllocTypeArray<FieldAsset::Emitter>(pFieldData->emitterCount);
	for (u32 emitterIndex = 0; emitterIndex < pFieldData->emitterCount; ++emitterIndex)
	{
		EmitterData* pEmitterData = binaryReader.ReadType<EmitterData>();

		ParticleControlData* paParticleControlData = binaryReader.ReadTypeArray<ParticleControlData>(pEmitterData->particleControlCount);
		
		ParticleUniformData* pParticleUniformData = m_pAllocator->AllocType<ParticleUniformData>();
		CopyControlToUniform(paParticleControlData, pEmitterData->particleControlCount, pEmitterData->globalAngle, pParticleUniformData);

		FieldAsset::Emitter* pEmitter = paEmitters + emitterIndex;
		pEmitter->pEmitterData = pEmitterData;
		pEmitter->paParticleControlData = paParticleControlData;
		pEmitter->pParticleUniformData = pParticleUniformData;
	}

	FieldAsset* pFieldAsset = m_pAllocator->AllocType<FieldAsset>();
	pFieldAsset->pFieldData = pFieldData;
	pFieldAsset->paEmitters = paEmitters;

	return pFieldAsset;
}

void AssetHandler::FixupAllAssets()
{
	for (pack::AssetIterator<FieldAsset> fieldIterator = m_pAssetContainer->GetIterator<FieldAsset>(); !fieldIterator.IsEnd(); fieldIterator.Next())
	{
		FieldAsset* pFieldAsset = fieldIterator.GetAsset();
		FieldData* pFieldData = pFieldAsset->pFieldData;
		imag::TextureAtlasAsset* pTextureAtlasAsset = m_pAssetContainer->FindAsset<imag::TextureAtlasAsset>(pFieldData->textureAtlasName);
		pFieldAsset->pTextureAtlasAsset = pTextureAtlasAsset;
		if (pTextureAtlasAsset != NULL)
		{
			for (u32 emitterIndex = 0; emitterIndex < pFieldData->emitterCount; ++emitterIndex)
			{
				FieldAsset::Emitter* pEmitter = pFieldAsset->paEmitters + emitterIndex;
				EmitterData* pEmitterData = pEmitter->pEmitterData;
				s32 textureRegionIndex = pTextureAtlasAsset->pTextureRegionIndexMap->FindIndex(pEmitterData->textureRegionName);
				if (textureRegionIndex >= 0)
				{
					imag::TextureAtlasAsset::TextureRegion* pAssetTextureRegion = pTextureAtlasAsset->paTextureRegions + textureRegionIndex;
					pEmitter->pTextureRegionData = pAssetTextureRegion->pTextureRegionData;
				}
			}
		}
	}
}

/*
* Private Instance
*/
void AssetHandler::CopyControlToUniform(ParticleControlData* paParticleControlData, u32 controlCount, f32 globalAngle, ParticleUniformData* pParticleUniformData)
{
	// set default values
	memset(pParticleUniformData, 0, sizeof(lct::part::ParticleUniformData));
	pParticleUniformData->aInitial[lct::part::PARTICLE_PROPERTY_TYPE_SIZE] = 1.0f;
	pParticleUniformData->aInitial[lct::part::PARTICLE_PROPERTY_TYPE_SCALE_X] = 1.0f;
	pParticleUniformData->aInitial[lct::part::PARTICLE_PROPERTY_TYPE_SCALE_Y] = 1.0f;
	pParticleUniformData->aInitial[lct::part::PARTICLE_PROPERTY_TYPE_ALPHA] = 1.0f;

	// overwrite with values from control data
	for (u32 controlIndex = 0; controlIndex < controlCount; ++controlIndex)
	{
		lct::part::ParticleControlData* pParticleControlData = paParticleControlData + controlIndex;
		lct::part::ParticlePropertyType propertyType = static_cast<lct::part::ParticlePropertyType>(pParticleControlData->propetyType);

		pParticleUniformData->aInitial[propertyType] = pParticleControlData->initial;
		pParticleUniformData->aVelocity[propertyType] = pParticleControlData->velocity;
		pParticleUniformData->aAcceleration[propertyType] = pParticleControlData->acceleration;
		pParticleUniformData->aVelocityFrameMin[propertyType] = pParticleControlData->velocityFrameRange.min;
		pParticleUniformData->aVelocityFrameMax[propertyType] = pParticleControlData->velocityFrameRange.max;
		pParticleUniformData->aAccelerationFrameMin[propertyType] = pParticleControlData->accelerationFrameRange.min;
		pParticleUniformData->aAccelerationFrameMax[propertyType] = pParticleControlData->accelerationFrameRange.max;
	}

	// calculate global direction
	pParticleUniformData->globalDirX = lct::foun::Cos(lct::foun::RadiansFromRotations(globalAngle));
	pParticleUniformData->globalDirY = lct::foun::Sin(lct::foun::RadiansFromRotations(globalAngle));
}

//namespace part
}
//namespace lct
}
