#include <part/part_fieldInstance.h>
#include <part/part_data.h>
#include <part/part_assets.h>
#include <part/part_arrays.h>

#include <grap/grap_device.h>
#include <grap/grap_data.h>
#include <grap/grap_resources.h>
#include <grap/grap_parameters.h>

#include <foun/foun_math.h>
#include <foun/foun_debug.h>

#include <string.h>

namespace lct
{
namespace part
{

/*
* Internal Constants
*/
static const foun::Vector2 QUAD_VERTEX_POSITIONS[QUAD_VERTEX_COUNT] =
{
	{ -0.5f, -0.5f },
	{ 0.5f, -0.5f },
	{ 0.5f, 0.5f },
	{ -0.5f, 0.5f }
};

/*
* Public Instance
*/
FieldInstance::FieldInstance()
: m_pEmitters(NULL)
, m_emitterCapacity(0)
, m_emitterCount(0)
, m_pParticles(NULL)
, m_particleCapacity(0)
, m_particleCount(0)
, m_pVertexData(NULL)
, m_pIndexData(NULL)
, m_vertexResourceIndex(0)
, m_pFieldAsset(NULL)
{
}

void FieldInstance::CreateStructure(u32 emitterCapacity, u32 particleCapacity, foun::Allocator* pAllocator)
{
	m_emitterCapacity = emitterCapacity;

	m_pEmitters = pAllocator->AllocTypeArray<Emitter>(m_emitterCapacity);
	
	m_particleCapacity = particleCapacity;
	
	m_pParticles = pAllocator->AllocTypeArray<Particle>(m_particleCapacity);
	
	u32 vertexCount = m_particleCapacity * QUAD_VERTEX_COUNT;
	m_pVertexData = pAllocator->AllocTypeArray<VertexData>(vertexCount);

	u32 indexCount = m_particleCapacity * QUAD_INDEX_COUNT;
	m_pIndexData = pAllocator->AllocTypeArray<IndexData>(indexCount);
	
	m_pVertexResources = pAllocator->AllocTypeArray<grap::VertexResource>(2);
	m_pIndexResource = pAllocator->AllocType<grap::IndexResource>();
}

void FieldInstance::AcquireResources(grap::Device* pGraphicsDevice)
{
	grap::VertexSetupParameters vertexSetupParameters;
	FillVertexSetupParameters(vertexSetupParameters, 0);
	pGraphicsDevice->AcquireVertexResource(vertexSetupParameters);
	FillVertexSetupParameters(vertexSetupParameters, 1);
	pGraphicsDevice->AcquireVertexResource(vertexSetupParameters);
	
	// index data does not change, set it once when aquiring
	for (u32 particleIndex = 0; particleIndex < m_particleCapacity; ++particleIndex)
	{
		u32 indexBaseIndex = particleIndex * QUAD_INDEX_COUNT;
		IndexData* pBaseIndex = m_pIndexData + indexBaseIndex;
		u32 vertexBaseIndex = particleIndex * QUAD_VERTEX_COUNT;

		pBaseIndex[0] = vertexBaseIndex + 0;
		pBaseIndex[1] = vertexBaseIndex + 1;
		pBaseIndex[2] = vertexBaseIndex + 2;
		pBaseIndex[3] = vertexBaseIndex + 2;
		pBaseIndex[4] = vertexBaseIndex + 3;
		pBaseIndex[5] = vertexBaseIndex + 0;
	}
	
	grap::IndexSetupParameters indexSetupParameters;
	FillIndexSetupParameters(indexSetupParameters);
	pGraphicsDevice->AcquireIndexResource(indexSetupParameters);
}

void FieldInstance::RefreshResources(grap::Device* pGraphicsDevice)
{
	// only refresh the current (non-drawn) vertex buffer
	grap::VertexSetupParameters vertexSetupParameters;
	FillVertexSetupParameters(vertexSetupParameters, m_vertexResourceIndex);
	pGraphicsDevice->RefreshVertexResource(vertexSetupParameters);
}

void FieldInstance::ReleaseResources(grap::Device* pGraphicsDevice)
{
	grap::VertexSetupParameters vertexSetupParameters;
	FillVertexSetupParameters(vertexSetupParameters, 0);
	pGraphicsDevice->ReleaseVertexResource(vertexSetupParameters);
	FillVertexSetupParameters(vertexSetupParameters, 1);
	pGraphicsDevice->ReleaseVertexResource(vertexSetupParameters);
	
	grap::IndexSetupParameters indexSetupParameters;
	FillIndexSetupParameters(indexSetupParameters);
	pGraphicsDevice->ReleaseIndexResource(indexSetupParameters);
}

void FieldInstance::SwapVertexResource()
{
	m_vertexResourceIndex = 1 - m_vertexResourceIndex;
}

void FieldInstance::ClearFieldAsset()
{
	m_pFieldAsset = NULL;
	
	m_emitterCount = 0;
	memset(m_pEmitters, 0, (sizeof(Emitter) * m_emitterCapacity));
	
	m_particleCount = 0;
	memset(m_pParticles, 0, (sizeof(Particle) * m_particleCapacity));
}

void FieldInstance::BindFieldAsset(const FieldAsset* pFieldAsset)
{
	FieldData* pFieldData = pFieldAsset->pFieldData;

	if (pFieldData->emitterCount > m_emitterCapacity)
	{
		return;
	}
	
	u32 assetParticleCount = 0;
	for (u32 assetEmitterIndex = 0; assetEmitterIndex < pFieldData->emitterCount; ++assetEmitterIndex)
	{
		const FieldAsset::Emitter& assetEmitter = pFieldAsset->pEmitters[assetEmitterIndex];
		const EmitterData* pEmitterData = assetEmitter.pEmitterData;
		assetParticleCount += pEmitterData->particleCount;
	}
	if (assetParticleCount > m_particleCapacity)
	{
		return;
	}

	m_pFieldAsset = pFieldAsset;

	m_emitterCount = pFieldData->emitterCount;
	m_particleCount = assetParticleCount;
	
	u32 emitterParticleIndex = 0;
	for (u32 emitterIndex = 0; emitterIndex < m_emitterCount; ++emitterIndex)
	{
		Emitter& emitter = m_pEmitters[emitterIndex];
		const EmitterData* pEmitterData = m_pFieldAsset->pEmitters[emitterIndex].pEmitterData;
		emitter.baseParticleIndex = emitterParticleIndex;
		
		emitterParticleIndex += pEmitterData->particleCount;
	}

	ResetEmitters();
}

void FieldInstance::SetRandomSeed(u32 seed)
{
	m_randomGenerator.SetSeed(seed);
}

void FieldInstance::ResetEmitters()
{
	for (u32 emitterIndex = 0; emitterIndex < m_emitterCount; ++emitterIndex)
	{
		Emitter& emitter = m_pEmitters[emitterIndex];
		
		emitter.frame = 0.0f;
		emitter.nextEmitFrame = 0.0f;
		emitter.nextEmitParticleIndex = 0;
		emitter.headLiveParticleIndex = 0;
		emitter.tailLiveParticleIndex = 0;
	}

	memset(m_pParticles, 0, (sizeof(Particle) * m_particleCapacity));
}

void FieldInstance::UpdateEmitters(f32 frameStep)
{	
	for (u32 emitterIndex = 0; emitterIndex < m_emitterCount; ++emitterIndex)
	{
		Emitter& emitter = m_pEmitters[emitterIndex];
		const EmitterData* pEmitterData = m_pFieldAsset->pEmitters[emitterIndex].pEmitterData;
		
		do
		{
			f32 frames = frameStep > pEmitterData->delayFrames ? pEmitterData->delayFrames : frameStep;

			emitter.frame += frames;
			if (emitter.frame >= emitter.nextEmitFrame)
			{
				u32 particleIndex = emitter.baseParticleIndex + emitter.nextEmitParticleIndex;
				Particle& particle = m_pParticles[particleIndex];
				particle.birthFrame = emitter.nextEmitFrame;
				particle.birthX = pEmitterData->x;
				particle.birthY = pEmitterData->y;

				f32 expelAngleDifference = pEmitterData->expelAngleRange.max - pEmitterData->expelAngleRange.min;
				f32 expelAngle = pEmitterData->expelAngleRange.min + (m_randomGenerator.GenerateFloat() * expelAngleDifference);
				particle.expelDirX = lct::foun::Cos(lct::foun::RadiansFromRotations(expelAngle));
				particle.expelDirY = lct::foun::Sin(lct::foun::RadiansFromRotations(expelAngle));
				
				for (u32 propertyIndex = 0; propertyIndex < PARTICLE_PROPERTY_COUNT; ++propertyIndex)
				{
					const Range& multiplierRange = pEmitterData->aParticleMultiplierRanges[propertyIndex];
					particle.aMultipliers[propertyIndex] = m_randomGenerator.GenerateFloat(multiplierRange.min, multiplierRange.max);
				}

				emitter.nextEmitFrame += pEmitterData->delayFrames;

				bool reverseOrder = pEmitterData->flags & (1 << EMITTER_FLAG_TYPE_REVERSE_ORDER);
				if (reverseOrder)
				{
					// set the next emit index					
					--emitter.nextEmitParticleIndex;
					if (emitter.nextEmitParticleIndex < 0)
					{
						emitter.nextEmitParticleIndex = pEmitterData->particleCount - 1;
					}

					// set the new tail index
					--emitter.tailLiveParticleIndex;
					++emitter.liveParticleCount;
					if (emitter.tailLiveParticleIndex < 0)
					{
						emitter.tailLiveParticleIndex = pEmitterData->particleCount - 1;
					}
					if (emitter.liveParticleCount > pEmitterData->particleCount)
					{
						// if the tail has gone past head, move the head
						--emitter.headLiveParticleIndex;
						--emitter.liveParticleCount;
						if (emitter.headLiveParticleIndex < 0)
						{
							emitter.headLiveParticleIndex = pEmitterData->particleCount - 1;
						}
					}
				}
				else
				{
					// set the next emit index
					++emitter.nextEmitParticleIndex;
					if (emitter.nextEmitParticleIndex >= pEmitterData->particleCount)
					{
						emitter.nextEmitParticleIndex = 0;
					}

					// set the new tail index
					++emitter.tailLiveParticleIndex;
					++emitter.liveParticleCount;
					if (emitter.tailLiveParticleIndex >= pEmitterData->particleCount)
					{
						emitter.tailLiveParticleIndex = 0;
					}
					if (emitter.liveParticleCount > pEmitterData->particleCount)
					{
						// if the tail has gone past head, move the head
						++emitter.headLiveParticleIndex;
						--emitter.liveParticleCount;
						if (emitter.headLiveParticleIndex >= pEmitterData->particleCount)
						{
							emitter.headLiveParticleIndex = 0;
						}
					}
				}
			}

			frameStep -= frames;
		} while (frameStep > 0.0f);
	}
}

void FieldInstance::ForceFrameOnEmitters(f32 frameStep)
{
	ResetEmitters();
	UpdateEmitters(frameStep);
}

void FieldInstance::UpdateParticles()
{
	u32 particleIndex = 0;
	for (u32 emitterIndex = 0; emitterIndex < m_emitterCount; ++emitterIndex)
	{
		Emitter& emitter = m_pEmitters[emitterIndex];
		const EmitterData* pEmitterData = m_pFieldAsset->pEmitters[emitterIndex].pEmitterData;
		
		for (u32 emitterParticleIndex = 0; emitterParticleIndex < pEmitterData->particleCount; ++emitterParticleIndex)
		{
			Particle& particle = m_pParticles[particleIndex];
			
			f32 lifeFrames = emitter.frame - particle.birthFrame;
			f32 aParticleProperties[PARTICLE_PROPERTY_COUNT];
			for (u32 propertyIndex = 0; propertyIndex < PARTICLE_PROPERTY_COUNT; ++propertyIndex)
			{
				const ParticleParameterData& particleParameterData = pEmitterData->aParticleParameterData[propertyIndex];
				f32 propertyFrames = foun::Clamp(lifeFrames, particleParameterData.frameRange.min, particleParameterData.frameRange.max) - particleParameterData.frameRange.min;
				aParticleProperties[propertyIndex] = particleParameterData.initial + (particleParameterData.velocity * propertyFrames) +
					(0.5f * particleParameterData.acceleration * propertyFrames * propertyFrames);
				aParticleProperties[propertyIndex] *= particle.aMultipliers[propertyIndex];
			}
			
			VertexData* pBaseVertexData = m_pVertexData + (particleIndex * QUAD_VERTEX_COUNT);
			for (u32 vertexIndex = 0; vertexIndex < QUAD_VERTEX_COUNT; ++vertexIndex)
			{
				foun::Vector2 vertexPosition = QUAD_VERTEX_POSITIONS[vertexIndex];
				vertexPosition.x *= aParticleProperties[PARTICLE_PROPERTY_SIZE];
				vertexPosition.y *= aParticleProperties[PARTICLE_PROPERTY_SIZE];
				
				foun::Matrix22 rotationMatrix;
				foun::Matrix22Rotate(rotationMatrix, aParticleProperties[PARTICLE_PROPERTY_ROTATION]);
				foun::TransformVector2(vertexPosition, vertexPosition, rotationMatrix);
				
				vertexPosition.x += particle.birthX;
				vertexPosition.y += particle.birthY;
				
				vertexPosition.x += (pEmitterData->globalDirX * aParticleProperties[PARTICLE_PROPERTY_GLOBAL_DISTANCE]);
				vertexPosition.y += (pEmitterData->globalDirY * aParticleProperties[PARTICLE_PROPERTY_GLOBAL_DISTANCE]);
				
				vertexPosition.x += (particle.expelDirX * aParticleProperties[PARTICLE_PROPERTY_EXPEL_DISTANCE]);
				vertexPosition.y += (particle.expelDirY * aParticleProperties[PARTICLE_PROPERTY_EXPEL_DISTANCE]);
				
				VertexData* pVertexData = pBaseVertexData + vertexIndex;
				
				pVertexData->x = vertexPosition.x;
				pVertexData->y = vertexPosition.y;
				
				pVertexData->s = 0.0f;
				pVertexData->t = 0.0f;
				
				foun::FloatColor3 color;
				const foun::FloatColor3& color0 = reinterpret_cast<const foun::FloatColor3&>(pEmitterData->color0);
				const foun::FloatColor3& color1 = reinterpret_cast<const foun::FloatColor3&>(pEmitterData->color1);
				foun::FloatColor3Lerp(color, color0, color1, aParticleProperties[PARTICLE_PROPERTY_COLOR_RATIO]);
				
				pVertexData->r = color.r;
				pVertexData->g = color.g;
				pVertexData->b = color.b;
				pVertexData->a = aParticleProperties[PARTICLE_PROPERTY_ALPHA];
			}
			
			++particleIndex;
		}
	}
}

/*
* Private Instance
*/
void FieldInstance::FillVertexSetupParameters(grap::VertexSetupParameters& vertexSetupParameters, u32 vertexResourceIndex)
{
	u32 vertexDataSize = sizeof(VertexData) * m_particleCapacity * QUAD_VERTEX_COUNT;

	vertexSetupParameters.pVertexResource = m_pVertexResources + vertexResourceIndex;
	vertexSetupParameters.pVertexBinary = m_pVertexData;
	vertexSetupParameters.vertexBinarySize = vertexDataSize;
	vertexSetupParameters.pAttributeDataArray = ATTRIBUTE_DATA_ARRAY;
	vertexSetupParameters.attributeCount = ATTRIBUTE_COUNT;
	vertexSetupParameters.dynamic = true;
}

void FieldInstance::FillIndexSetupParameters(grap::IndexSetupParameters& indexSetupParameters)
{
	u32 indexDataSize = sizeof(IndexData) * m_particleCapacity * QUAD_INDEX_COUNT;

	indexSetupParameters.pIndexResource = m_pIndexResource;
	indexSetupParameters.pIndexBinary = m_pIndexData;
	indexSetupParameters.indexBinarySize = indexDataSize;
	indexSetupParameters.dynamic = true;
}

//namespace part
}
//namespace lct
}
