#ifndef PART_DRAW_CONTEXT_H
#define PART_DRAW_CONTEXT_H

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
struct ShaderSetupParameters;
}

namespace part
{

class FieldInstance;

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
	void ActivateFieldInstance(FieldInstance& fieldInstance);
	void DeactivateFieldInstance(FieldInstance& fieldInstance);
	void ActivateProjectionTransform(const foun::Matrix44& projectionTransform);
	void ActivateWorldTransform(const foun::Matrix32& worldTransform);

	void DrawField(FieldInstance& fieldInstance);

protected:
	void FillShaderSetupParameters(grap::ShaderSetupParameters& shaderSetupParameters);

	foun::Allocator* m_pAllocator;

	grap::Device* m_pGraphicsDevice;

	void* m_pVertexShaderBinary;
	void* m_pFragmentShaderBinary;
	grap::ShaderResource* m_pShaderResource;
	grap::UniformResource* m_pUniformResourceArray;
};

//namespace part
}
//namespace lct
}

#endif//PART_DRAW_CONTEXT_H
