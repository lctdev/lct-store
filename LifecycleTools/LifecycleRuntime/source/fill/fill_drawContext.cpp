#include <fill/fill_drawContext.h>
#include <fill/fill_data.h>
#include <fill/fill_constants.h>
#include <fill/fill_arrays.h>

#include <grap/grap_device.h>
#include <grap/grap_data.h>
#include <grap/grap_resources.h>
#include <grap/grap_parameters.h>

#include <imag/imag_resources.h>

#include <foun/foun_vector.h>
#include <foun/foun_debug.h>
#include <foun/foun_graphicsDebug.h>
#include <foun/foun_string.h>

namespace lct
{
namespace fill
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
, m_pQuadVertexResource(NULL)
, m_pQuadIndexResource(NULL)
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

	m_pQuadVertexResource = m_pAllocator->AllocType<grap::VertexResource>();
	m_pQuadIndexResource = m_pAllocator->AllocType<grap::IndexResource>();
}

void DrawContext::AcquireResources()
{
	LCT_TRACE("fill::DrawContext::Acquireresources\n");

	grap::ShaderResourceParameters shaderResourceParameters;
	FillShaderResourceParameters(shaderResourceParameters);
	m_pGraphicsDevice->AcquireShaderResources(shaderResourceParameters);

	grap::VertexResourceParameters vertexResourceParameters;
	grap::IndexResourceParameters indexResourceParameters;
	FillQuadResourceParameters(vertexResourceParameters, indexResourceParameters);
	m_pGraphicsDevice->AcquireVertexResource(vertexResourceParameters);
	m_pGraphicsDevice->AcquireIndexResource(indexResourceParameters);
}

void DrawContext::ReleaseResources()
{
	grap::ShaderResourceParameters shaderResourceParameters;
	FillShaderResourceParameters(shaderResourceParameters);
	m_pGraphicsDevice->ReleaseShaderResources(shaderResourceParameters);

	grap::VertexResourceParameters vertexResourceParameters;
	grap::IndexResourceParameters indexResourceParameters;
	FillQuadResourceParameters(vertexResourceParameters, indexResourceParameters);
	m_pGraphicsDevice->ReleaseVertexResource(vertexResourceParameters);
	m_pGraphicsDevice->ReleaseIndexResource(indexResourceParameters);
}

void DrawContext::ActivateRenderState()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	FOUN_TRACE_GL_ERROR();
}

void DrawContext::ActivateShader()
{
	m_pGraphicsDevice->ActivateShader(m_pShaderResource);
}

void DrawContext::ActivateQuad()
{
	m_pGraphicsDevice->ActivateVertex(m_pQuadVertexResource);

	m_pGraphicsDevice->ActivateIndex(m_pQuadIndexResource);
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

void DrawContext::DrawRect(const foun::RectCentered& rect, const foun::FloatColor& color)
{
	const grap::UniformResource* pPlacementUniformResource = m_pUniformResourceArray + UNIFORM_PLACEMENT;
	const grap::UniformResource* pModulateColorUniformResource = m_pUniformResourceArray + UNIFORM_MODULATE_COLOR;

	foun::Vector4 placement;
	placement.x = rect.width;
	placement.y = rect.height;
	placement.z = rect.x;
	placement.w = rect.y;

	m_pGraphicsDevice->ActivateUniform(pPlacementUniformResource, placement);
	m_pGraphicsDevice->ActivateUniform(pModulateColorUniformResource, color);

	glDrawElements(GL_TRIANGLES, QUAD_INDEX_COUNT, GL_UNSIGNED_BYTE, NULL);
}

/*
 * Protected Instance
 */
void DrawContext::FillShaderResourceParameters(grap::ShaderResourceParameters& shaderResourceParameters)
{
	shaderResourceParameters.pShaderResource = m_pShaderResource;
	shaderResourceParameters.pVertexShaderBinary = m_pVertexShaderBinary;
	shaderResourceParameters.pFragmentShaderBinary = m_pFragmentShaderBinary;
	shaderResourceParameters.pAttributeDataArray = ATTRIBUTE_DATA_ARRAY;
	shaderResourceParameters.attributeCount = ATTRIBUTE_COUNT;
	shaderResourceParameters.pUniformResourceArray = m_pUniformResourceArray;
	shaderResourceParameters.pUniformDataArray = UNIFORM_DATA_ARRAY;
	shaderResourceParameters.uniformCount = UNIFORM_COUNT;
}

void DrawContext::FillQuadResourceParameters(grap::VertexResourceParameters& vertexResourceParameters, grap::IndexResourceParameters& indexResourceParameters)
{
	vertexResourceParameters.pVertexResource = m_pQuadVertexResource;
	vertexResourceParameters.pVertexBinary = QUAD_VERTEX_DATA_ARRAY;
	vertexResourceParameters.vertexBinarySize = sizeof(QUAD_VERTEX_DATA_ARRAY);
	vertexResourceParameters.pAttributeDataArray = ATTRIBUTE_DATA_ARRAY;
	vertexResourceParameters.attributeCount = ATTRIBUTE_COUNT;
	vertexResourceParameters.dynamic = false;

	indexResourceParameters.pIndexResource = m_pQuadIndexResource;
	indexResourceParameters.pIndexBinary = QUAD_INDEX_DATA_ARRAY;
	indexResourceParameters.indexBinarySize = sizeof(QUAD_INDEX_DATA_ARRAY);
	indexResourceParameters.dynamic = false;
}

//namespace spri
}
//namespace lct
}
