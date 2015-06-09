#ifndef FONT_DRAW_CONTEXT_H
#define FONT_DRAW_CONTEXT_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>
#include <foun/foun_matrix.h>

#include <grap/grap_parameters.h>

namespace lct
{
namespace grap
{
class Device;
struct ShaderResource;
struct UniformResource;
struct VertexResource;
struct IndexResource;
struct ShaderResourceParameters;
struct VertexResourceParameters;
}

namespace font
{

struct VertexData;
class SymbolBuffer;

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
	void ActivateSymbolBuffer(SymbolBuffer& symbolBuffer);
	void ActivateProjectionTransform(const foun::Matrix44& projectionTransform);
	void ActivateWorldTransform(const foun::Matrix32& worldTransform);

	void DrawSymbolBuffer(SymbolBuffer& symbolBuffer);

protected:
	void FillShaderResourceParameters(grap::ShaderResourceParameters& shaderResourceParameters);
	void FillVertexResourceParameters(grap::VertexResourceParameters& vertexResourceParameters);

	foun::Allocator* m_pAllocator;

	grap::Device* m_pGraphicsDevice;

	void* m_pVertexShaderBinary;
	void* m_pFragmentShaderBinary;
	grap::ShaderResource* m_pShaderResource;
	grap::UniformResource* m_pUniformResourceArray;

	grap::VertexResource* m_pQuadVertexResource;
	grap::IndexResource* m_pQuadIndexResource;
};

//namespace spri
}
//namespace lct
}

#endif//FONT_DRAW_CONTEXT_H
