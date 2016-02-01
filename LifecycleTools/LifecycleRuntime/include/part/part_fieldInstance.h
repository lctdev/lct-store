#ifndef PART_FIELD_INSTANCE_H
#define PART_FIELD_INSTANCE_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>

namespace lct
{
namespace grap
{
class Device;
struct VertexResource;
struct IndexResource;
struct VertexSetupParameters;
struct IndexSetupParameters;
}

namespace util
{
	class IntegerIndexMap;
}

namespace imag
{
	struct TextureAsset;
}

namespace part
{

struct VertexData;
typedef u16 IndexData;

struct FieldAsset;
struct EmitterData;

class FieldInstance
{
public:
	FieldInstance();

	void CreateStructure(u32 emitterCapacity, u32 particleCapacity, foun::Allocator* pAllocator);

	void AcquireResources(grap::Device* pGraphicsDevice);
	void RefreshResources(grap::Device* pGraphicsDevice);
	void ReleaseResources(grap::Device* pGraphicsDevice);
	void SwapVertexResource();

	void ClearFieldAsset();
	void BindFieldAsset(const FieldAsset* pFieldAsset);
	
	void ResetEmitters();
	void UpdateEmitters(f32 frameStep);
	void UpdateParticles();

protected:	
	void FillVertexSetupParameters(grap::VertexSetupParameters& vertexSetupParameters, u32 vertexResourceIndex);
	void FillIndexSetupParameters(grap::IndexSetupParameters& indexSetupParameters);

	struct Particle;
	struct Emitter
	{
		u32 baseParticleIndex;
		f32 frame;
		f32 nextEmitFrame;
		u32 nextEmitParticleIndex;
	};
	Emitter* m_pEmitters;
	u32 m_emitterCapacity;
	u32 m_emitterCount;
	
	struct Particle
	{
		f32 birthFrame;
		f32 birthX;
		f32 birthY;
		f32 expelDirX;
		f32 expelDirY;
	};
	Particle* m_pParticles;
	u32 m_particleCapacity;
	u32 m_particleCount;
	
	VertexData* m_pVertexData;
	IndexData* m_pIndexData;
	grap::VertexResource* m_pVertexResources;
	grap::IndexResource* m_pIndexResource;
	u32 m_vertexResourceIndex;
	
	const FieldAsset* m_pFieldAsset;

	friend class DrawContext;
};

//namespace part
}
//namespace lct
}

#endif//PART_FIELD_INSTANCE_H
