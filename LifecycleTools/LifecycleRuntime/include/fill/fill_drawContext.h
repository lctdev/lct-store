#ifndef FILL_DRAW_CONTEXT_H
#define FILL_DRAW_CONTEXT_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>
#include <foun/foun_matrix.h>
#include <foun/foun_rect.h>
#include <foun/foun_color.h>

namespace lct
{
namespace grap
{
class Device;
struct ShaderResource;
struct UniformResource;
struct VertexResource;
struct IndexResource;
struct ShaderSetupParameters;
struct VertexSetupParameters;
struct IndexSetupParameters;
}

namespace fill
{

struct VertexData;
struct QuadResource;
class FigureInstance;

class DrawContext
{
public:
	DrawContext();

	void SetAllocator(foun::Allocator* pAllocator);
	void SetGraphicsDevice(grap::Device* pGraphicsDevice);
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

	void DrawRect(const foun::RectCentered& rect, const foun::FloatColor& color);
	void DrawRect(const foun::RectEdges& rect, const foun::FloatColor& color);

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

//namespace fill
}
//namespace lct
}

#endif//FILL_DRAW_CONTEXT_H
