#ifndef SOUN_PLAY_CONTEXT_H
#define SOUN_PLAY_CONTEXT_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>

namespace lct
{
namespace audi
{
class Device;
}

namespace soun
{

class ClipInstance;

class PlayContext
{
public:
	PlayContext();

	void SetAllocator(foun::Allocator* pAllocator);
	void SetAudioDevice(audi::Device* pAudioDevice);
	void SetShaderBinaries(void* pVertexShaderBinary, void* pFragmentShaderBinary);

	void CreateResources();
	void AcquireResources();
	void ReleaseResources();

	void ActivateRenderState();
	void ActivateShader();
	void ActivateQuad();
	void DeactivateQuad();
	void ActivateProjectionTransform(const foun::Matrix44& projectionTransform);
	void ActivateWorldTransform(const foun::Matrix32& worldTransform);

	void DrawFigure(FigureInstance& figureInstance);

protected:
	void FillShaderSetupParameters(grap::ShaderSetupParameters& shaderSetupParameters);
	void FillQuadSetupParameters(grap::VertexSetupParameters& vertexSetupParameters, grap::IndexSetupParameters& indexSetupParameters);

	foun::Allocator* m_pAllocator;

	grap::Device* m_pGraphicsDevice;

	void* m_pVertexShaderBinary;
	void* m_pFragmentShaderBinary;
	grap::ShaderResource* m_pShaderResource;
	grap::UniformResource* m_pUniformResourceArray;

	grap::VertexResource* m_pQuadVertexResource;
	grap::IndexResource* m_pQuadIndexResource;
};

//namespace soun
}
//namespace lct
}

#endif//SOUN_PLAY_CONTEXT_H
