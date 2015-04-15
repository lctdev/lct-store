#ifndef SPRI_DRAW_CONTEXT_H
#define SPRI_DRAW_CONTEXT_H

#include <foun/foun_primitives.h>
#include <foun/foun_allocator.h>
#include <foun/foun_matrix.h>
#include <foun/foun_rect.h>
#include <foun/foun_color.h>

namespace lct
{
namespace shad
{
class ResourceHandler;
struct ShaderResource;
struct AttributeResource;
struct UniformResource;
}
namespace imag
{
struct TextureResource;
}

namespace spri
{

struct VertexData;
struct QuadResource;
class FigureInstance;

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
	void ActivateQuad();
	void ActivateProjectionTransform(const foun::Matrix44& projectionTransform);
	void ActivateWorldTransform(const foun::Matrix32& worldTransform);

	void DrawFigure(FigureInstance& figureInstance);

protected:
	foun::Allocator* m_pAllocator;

	void AcquireUniformResource(shad::UniformResource* pUniformResource, const char* pName);

	shad::ResourceHandler* m_pShaderResourceHandler;
	void* m_pVertexShaderBinary;
	void* m_pFragmentShaderBinary;
	shad::ShaderResource* m_pShaderResource;
	shad::AttributeResource* m_pAttributeResourceArray;
	shad::UniformResource* m_pPlacementUniformResource;
	shad::UniformResource* m_pJointTransformUniformResource;
	shad::UniformResource* m_pWorldTransformUniformResource;
	shad::UniformResource* m_pProjectionTransformUniformResource;
	shad::UniformResource* m_pModulateColorUniformResource;
	shad::UniformResource* m_pInterpolateColorUniformResource;
	shad::UniformResource* m_pTextureUniformResource;

	QuadResource* m_pQuadResource;
};

//namespace spri
}
//namespace lct
}

#endif//SPRI_DRAW_CONTEXT_H
