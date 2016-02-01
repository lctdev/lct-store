#include <part/part_drawContext.h>
#include <part/part_data.h>
#include <part/part_fieldInstance.h>
#include <part/part_constants.h>
#include <part/part_arrays.h>

#include <grap/grap_device.h>
#include <grap/grap_data.h>
#include <grap/grap_resources.h>
#include <grap/grap_parameters.h>

#include <util/util_indexMap.h>

#include <foun/foun_debug.h>
#include <foun/foun_string.h>

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
DrawContext::DrawContext()
: m_pAllocator(NULL)
, m_pGraphicsDevice(NULL)
, m_pShaderResource(NULL)
, m_pUniformResourceArray(NULL)
, m_pVertexShaderBinary(NULL)
, m_pFragmentShaderBinary(NULL)
{
}

void DrawContext::SetAllocator(lct::foun::Allocator* pAllocator)
{
	m_pAllocator = pAllocator;
}

void DrawContext::SetGraphicsDevice(grap::Device* pGraphicsDevice)
{
	m_pGraphicsDevice = pGraphicsDevice;
}

void DrawContext::SetShaderBinaries(void* pVertexShaderBinary, void* pFragmentShaderBinary)
{ 
	m_pVertexShaderBinary = pVertexShaderBinary;
	m_pFragmentShaderBinary = pFragmentShaderBinary;
}

void DrawContext::CreateResources()
{
	m_pShaderResource = m_pAllocator->AllocType<grap::ShaderResource>();
	m_pUniformResourceArray = m_pAllocator->AllocTypeArray<grap::UniformResource>(UNIFORM_COUNT);
}

void DrawContext::AcquireResources()
{
	LCT_TRACE("part::DrawContext::Acquireresources\n");

	grap::ShaderSetupParameters shaderSetupParameters;
	FillShaderSetupParameters(shaderSetupParameters);
	m_pGraphicsDevice->AcquireShaderResources(shaderSetupParameters);
}

void DrawContext::ReleaseResources()
{
	grap::ShaderSetupParameters shaderSetupParameters;
	FillShaderSetupParameters(shaderSetupParameters);
	m_pGraphicsDevice->ReleaseShaderResources(shaderSetupParameters);
}

void DrawContext::ActivateRenderState()
{
	grap::RenderStateParameters renderStateParameters;
	renderStateParameters.enableBlend = true;
	m_pGraphicsDevice->ActivateRenderState(renderStateParameters);
}

void DrawContext::ActivateShader()
{
	m_pGraphicsDevice->ActivateShader(m_pShaderResource);
}

void DrawContext::ActivateFieldInstance(FieldInstance& fieldInstance)
{
	grap::VertexResource* pVertexResource = fieldInstance.m_pVertexResources + fieldInstance.m_vertexResourceIndex;
	m_pGraphicsDevice->ActivateVertex(pVertexResource);

	m_pGraphicsDevice->ActivateIndex(fieldInstance.m_pIndexResource);
}

void DrawContext::DeactivateFieldInstance(FieldInstance& fieldInstance)
{
	grap::VertexResource* pVertexResource = fieldInstance.m_pVertexResources + fieldInstance.m_vertexResourceIndex;
	m_pGraphicsDevice->DeactivateVertex(pVertexResource);
}

void DrawContext::ActivateProjectionTransform(const foun::Matrix44& projectionTransform)
{
	const grap::UniformResource* pProjectionTransformUniformResource = m_pUniformResourceArray + UNIFORM_PROJECTION_TRANSFORM;

	m_pGraphicsDevice->ActivateUniform(pProjectionTransformUniformResource, projectionTransform);
}

void DrawContext::ActivateWorldTransform(const foun::Matrix32& worldTransform)
{
	foun::Matrix33 tempWorldMatrix;
	foun::Matrix33FromMatrix32(tempWorldMatrix, worldTransform);
	const grap::UniformResource* pWorldTransformUniformResource = m_pUniformResourceArray + UNIFORM_WORLD_TRANSFORM;

	m_pGraphicsDevice->ActivateUniform(pWorldTransformUniformResource, tempWorldMatrix);	
}

void DrawContext::DrawField(FieldInstance& fieldInstance)
{
	//for (u32 emitterIndex = 0; emitterIndex < fieldInstance.m_emitterCount; ++emitterIndex)
	//{
	//	FieldInstance::Emitter& emitter = fieldInstance.m_pEmitters[emitterIndex];
		
	//}
	
	u32 indexCount = fieldInstance.m_particleCount * QUAD_INDEX_COUNT;
	m_pGraphicsDevice->Draw(indexCount, 0, grap::INDEX_TYPE_U16);
}

/*
 * Protected Instance
 */
void DrawContext::FillShaderSetupParameters(grap::ShaderSetupParameters& shaderSetupParameters)
{
	shaderSetupParameters.pShaderResource = m_pShaderResource;
	shaderSetupParameters.pVertexShaderBinary = m_pVertexShaderBinary;
	shaderSetupParameters.pFragmentShaderBinary = m_pFragmentShaderBinary;
	shaderSetupParameters.pAttributeDataArray = ATTRIBUTE_DATA_ARRAY;
	shaderSetupParameters.attributeCount = ATTRIBUTE_COUNT;
	shaderSetupParameters.pUniformResourceArray = m_pUniformResourceArray;
	shaderSetupParameters.pUniformDataArray = UNIFORM_DATA_ARRAY;
	shaderSetupParameters.uniformCount = UNIFORM_COUNT;
}

//namespace part
}
//namespace lct
}
