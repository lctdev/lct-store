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
	memset(m_pParticles, 0, (sizeof(Particle) * m_particleCapacity));

	for (u32 emitterIndex = 0; emitterIndex < m_emitterCount; ++emitterIndex)
	{
		Emitter& emitter = m_pEmitters[emitterIndex];
		const EmitterData* pEmitterData = m_pFieldAsset->pEmitters[emitterIndex].pEmitterData;
		
		emitter.frame = 0.0f;
		emitter.nextEmitFrame = 0.0f;
		emitter.headLiveParticleIndex = 0;
		emitter.tailLiveParticleIndex = 0;
		emitter.liveParticleCount = 0;

		SpawnParticles(emitterIndex);

		emitter.nextEmitFrame = pEmitterData->delayFrames;
	}	
}

void FieldInstance::UpdateEmitters(f32 frameStep)
{	
	for (u32 emitterIndex = 0; emitterIndex < m_emitterCount; ++emitterIndex)
	{
		Emitter& emitter = m_pEmitters[emitterIndex];
		const EmitterData* pEmitterData = m_pFieldAsset->pEmitters[emitterIndex].pEmitterData;
			
		// spawn new particles
		do
		{
			f32 frames = frameStep > pEmitterData->delayFrames ? pEmitterData->delayFrames : frameStep;

			emitter.frame += frames;
			f32 frameDuration = pEmitterData->frameDuration;
			if ((frameDuration <= 0.0f) || (emitter.frame < frameDuration))
			{
				if (emitter.frame >= emitter.nextEmitFrame)
				{
					SpawnParticles(emitterIndex);

					emitter.nextEmitFrame += pEmitterData->delayFrames;
				}
			}

			frameStep -= frames;
		} while (frameStep > 0.0f);

		// cull old particles
		CullParticles(emitterIndex);
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
			f32 aParticleProperties[PARTICLE_PROPERTY_TYPE_COUNT];
			for (u32 propertyIndex = 0; propertyIndex < PARTICLE_PROPERTY_TYPE_COUNT; ++propertyIndex)
			{
				const ParticleParameterData& particleParameterData = pEmitterData->aParticleParameterData[propertyIndex];
				f32 velocityFrames = foun::Clamp(lifeFrames, particleParameterData.velocityFrameRange.min, particleParameterData.velocityFrameRange.max) - particleParameterData.velocityFrameRange.min;
				f32 accelerationFrames = foun::Clamp(lifeFrames, particleParameterData.accelerationFrameRange.min, particleParameterData.accelerationFrameRange.max) - particleParameterData.accelerationFrameRange.min;
				aParticleProperties[propertyIndex] = particleParameterData.initial + (particleParameterData.velocity * velocityFrames) +
					(0.5f * particleParameterData.acceleration * accelerationFrames * accelerationFrames);
				aParticleProperties[propertyIndex] *= particle.aMultipliers[propertyIndex];
			}
			
			VertexData* pBaseVertexData = m_pVertexData + (particleIndex * QUAD_VERTEX_COUNT);
			for (u32 vertexIndex = 0; vertexIndex < QUAD_VERTEX_COUNT; ++vertexIndex)
			{
				foun::Vector2 vertexPosition = QUAD_VERTEX_POSITIONS[vertexIndex];
				vertexPosition.x *= aParticleProperties[PARTICLE_PROPERTY_TYPE_SIZE];
				vertexPosition.y *= aParticleProperties[PARTICLE_PROPERTY_TYPE_SIZE];
				
				f32 rotation = particle.birthRotation + aParticleProperties[PARTICLE_PROPERTY_TYPE_ROTATION];
				foun::Matrix22 rotationMatrix;
				foun::Matrix22Rotate(rotationMatrix, rotation);
				foun::TransformVector2(vertexPosition, vertexPosition, rotationMatrix);
				
				vertexPosition.x += particle.birthX;
				vertexPosition.y += particle.birthY;
				
				vertexPosition.x += (pEmitterData->globalDirX * aParticleProperties[PARTICLE_PROPERTY_TYPE_GLOBAL_DISTANCE]);
				vertexPosition.y += (pEmitterData->globalDirY * aParticleProperties[PARTICLE_PROPERTY_TYPE_GLOBAL_DISTANCE]);

				f32 expelAngle = particle.expelAngle;
				expelAngle += aParticleProperties[PARTICLE_PROPERTY_TYPE_EXPEL_ANGLE];

				foun::Vector2 expelDirection;
				expelDirection.x = lct::foun::Cos(lct::foun::RadiansFromRotations(expelAngle));
				expelDirection.y = lct::foun::Sin(lct::foun::RadiansFromRotations(expelAngle));
				
				vertexPosition.x += (expelDirection.x * aParticleProperties[PARTICLE_PROPERTY_TYPE_EXPEL_DISTANCE]);
				vertexPosition.y += (expelDirection.y * aParticleProperties[PARTICLE_PROPERTY_TYPE_EXPEL_DISTANCE]);
				
				VertexData* pVertexData = pBaseVertexData + vertexIndex;
				
				pVertexData->x = vertexPosition.x;
				pVertexData->y = vertexPosition.y;
				
				pVertexData->s = 0.0f;
				pVertexData->t = 0.0f;
				
				foun::FloatColor3 color;
				const foun::FloatColor3& color0 = reinterpret_cast<const foun::FloatColor3&>(pEmitterData->color0);
				const foun::FloatColor3& color1 = reinterpret_cast<const foun::FloatColor3&>(pEmitterData->color1);
				foun::FloatColor3Lerp(color, color0, color1, aParticleProperties[PARTICLE_PROPERTY_TYPE_COLOR_RATIO]);
				
				pVertexData->r = color.r;
				pVertexData->g = color.g;
				pVertexData->b = color.b;
				pVertexData->a = aParticleProperties[PARTICLE_PROPERTY_TYPE_ALPHA];
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

void FieldInstance::SpawnParticles(u32 emitterIndex)
{
	Emitter& emitter = m_pEmitters[emitterIndex];
	const EmitterData* pEmitterData = m_pFieldAsset->pEmitters[emitterIndex].pEmitterData;

	bool reverseOrder = pEmitterData->flags & (1 << EMITTER_FLAG_TYPE_REVERSE_ORDER);

	s32 headLiveParticleIndex = emitter.headLiveParticleIndex;
	s32 tailLiveParticleIndex = emitter.tailLiveParticleIndex;
	u32 liveParticleCount = emitter.liveParticleCount;

	for (u32 emitIndex = 0; emitIndex < pEmitterData->emitCount; ++emitIndex)
	{
		u32 particleIndex = emitter.baseParticleIndex + tailLiveParticleIndex;
		Particle& particle = m_pParticles[particleIndex];
		particle.birthFrame = emitter.nextEmitFrame;
		foun::Vector2 birthPosition;
		birthPosition.x = pEmitterData->x;
		birthPosition.y = pEmitterData->y;
		f32 birthAngle = 0.0f;
		switch (pEmitterData->shapeType)
		{
		case lct::part::EMITTER_SHAPE_TYPE_CIRCLE:
		{
			birthAngle = emitIndex * (1.0f / pEmitterData->emitCount);
			f32 radius = pEmitterData->shapeSpanA / 2.0f;
			birthPosition.x += lct::foun::Cos(lct::foun::RadiansFromRotations(birthAngle)) * radius;
			birthPosition.y += lct::foun::Sin(lct::foun::RadiansFromRotations(birthAngle)) * radius;
		}
		break;
		}

		particle.birthX = birthPosition.x;
		particle.birthY = birthPosition.y;

		bool rotateOutward = pEmitterData->flags & (1 << lct::part::EMITTER_FLAG_TYPE_ROTATE_OUTWARD);
		if (rotateOutward)
		{
			particle.birthRotation = birthAngle;
		}
		else
		{
			particle.birthRotation = 0.0f;
		}

		f32 expelAngleDifference = pEmitterData->expelAngleRange.max - pEmitterData->expelAngleRange.min;
		f32 expelAngle = pEmitterData->expelAngleRange.min + (m_randomGenerator.GenerateFloat() * expelAngleDifference);
		bool expelOutward = pEmitterData->flags & (1 << lct::part::EMITTER_FLAG_TYPE_EXPEL_OUTWARD);
		if (expelOutward)
		{
			expelAngle += birthAngle;
		}

		particle.expelAngle = expelAngle;

		for (u32 propertyIndex = 0; propertyIndex < PARTICLE_PROPERTY_TYPE_COUNT; ++propertyIndex)
		{
			const Range& multiplierRange = pEmitterData->aParticleMultiplierRanges[propertyIndex];
			particle.aMultipliers[propertyIndex] = m_randomGenerator.GenerateFloat(multiplierRange.min, multiplierRange.max);
		}

		if (reverseOrder)
		{
			// set the new tail index
			--tailLiveParticleIndex;
			++liveParticleCount;
			if (tailLiveParticleIndex < 0)
			{
				tailLiveParticleIndex = pEmitterData->particleCount - 1;
			}
			if (liveParticleCount > pEmitterData->particleCount)
			{
				// if the tail has gone past head, move the head
				--headLiveParticleIndex;
				--liveParticleCount;
				if (headLiveParticleIndex < 0)
				{
					headLiveParticleIndex = pEmitterData->particleCount - 1;
				}
			}
		}
		else
		{
			// set the new tail index
			++tailLiveParticleIndex;
			++liveParticleCount;
			if (tailLiveParticleIndex >= pEmitterData->particleCount)
			{
				tailLiveParticleIndex = 0;
			}
			if (liveParticleCount > pEmitterData->particleCount)
			{
				// if the tail has gone past head, move the head
				++headLiveParticleIndex;
				--liveParticleCount;
				if (headLiveParticleIndex >= pEmitterData->particleCount)
				{
					headLiveParticleIndex = 0;
				}
			}
		}
	}

	emitter.headLiveParticleIndex = headLiveParticleIndex;
	emitter.tailLiveParticleIndex = tailLiveParticleIndex;
	emitter.liveParticleCount = liveParticleCount;
}

void FieldInstance::CullParticles(u32 emitterIndex)
{
	Emitter& emitter = m_pEmitters[emitterIndex];
	const EmitterData* pEmitterData = m_pFieldAsset->pEmitters[emitterIndex].pEmitterData;

	bool reverseOrder = pEmitterData->flags & (1 << EMITTER_FLAG_TYPE_REVERSE_ORDER);

	s32 headLiveParticleIndex = emitter.headLiveParticleIndex;
	s32 tailLiveParticleIndex = emitter.tailLiveParticleIndex;
	u32 liveParticleCount = emitter.liveParticleCount;

	f32 deadFrame = emitter.frame - pEmitterData->particleFrameLifetime;
	while (liveParticleCount > 0)
	{
		u32 particleIndex = emitter.baseParticleIndex + headLiveParticleIndex;
		Particle& particle = m_pParticles[particleIndex];

		if (particle.birthFrame > deadFrame)
		{
			// particles from here on are alive
			break;
		}
		else
		{
			// particle is dead
			if (reverseOrder)
			{
				// set the new head index
				--headLiveParticleIndex;
				--liveParticleCount;
				if (headLiveParticleIndex < 0)
				{
					headLiveParticleIndex = pEmitterData->particleCount - 1;
				}
			}
			else
			{
				// set the new head index
				++headLiveParticleIndex;
				--liveParticleCount;
				if (headLiveParticleIndex >= pEmitterData->particleCount)
				{
					headLiveParticleIndex = 0;
				}
			}
		}
	}

	emitter.headLiveParticleIndex = headLiveParticleIndex;
	emitter.tailLiveParticleIndex = tailLiveParticleIndex;
	emitter.liveParticleCount = liveParticleCount;
}

//namespace part
}
//namespace lct
}
