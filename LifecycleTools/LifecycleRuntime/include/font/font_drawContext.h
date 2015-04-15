#ifndef FONT_DRAW_CONTEXT_H
#define FONT_DRAW_CONTEXT_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>
#include <foun/foun_matrix.h>

namespace lct
{
namespace shad
{
class ResourceHandler;
struct ShaderResource;
struct AttributeResource;
struct UniformResource;
}

namespace font
{

struct VertexData;
struct QuadResource;
class SymbolBuffer;

class DrawContext
{
public:
	DrawContext();

	void SetAllocator(foun::Allocator* pAllocator);
	void SetShaderResourceHandler(shad::ResourceHandler* pShaderResourceHandler);
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
	foun::Allocator* m_pAllocator;

	void AcquireUniformResource(shad::UniformResource* pUniformResource, const char* pName);

	shad::ResourceHandler* m_pShaderResourceHandler;
	void* m_pVertexShaderBinary;
	void* m_pFragmentShaderBinary;
	shad::ShaderResource* m_pShaderResource;
	shad::AttributeResource* m_pAttributeResourceArray;
	shad::UniformResource* m_pProjectionTransformUniformResource;
	shad::UniformResource* m_pWorldTransformUniformResource;
	shad::UniformResource* m_pModulateColorUniformResource;
	shad::UniformResource* m_pTextureUniformResource;
};

//namespace spri
}
//namespace lct
}

#endif//FONT_DRAW_CONTEXT_H
