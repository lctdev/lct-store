#include <font/font_drawContext.h>
#include <font/font_assets.h>
#include <font/font_data.h>
#include <font/font_symbolBuffer.h>
#include <font/font_constants.h>
#include <font/font_arrays.h>

#include <grap/grap_device.h>
#include <grap/grap_data.h>
#include <grap/grap_resources.h>

#include <imag/imag_assets.h>
#include <imag/imag_resources.h>

#include <util/util_indexMap.h>

#include <foun/foun_debug.h>
#include <foun/foun_graphicsDebug.h>
#include <foun/foun_string.h>

namespace lct
{
namespace font
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
	LCT_TRACE("font::DrawContext::Acquireresources\n");

	grap::ShaderResourceParameters shaderResourceParameters;
	FillShaderResourceParameters(shaderResourceParameters);
	m_pGraphicsDevice->AcquireShaderResources(shaderResourceParameters);
}

void DrawContext::ReleaseResources()
{
	grap::ShaderResourceParameters shaderResourceParameters;
	FillShaderResourceParameters(shaderResourceParameters);
	m_pGraphicsDevice->ReleaseShaderResources(shaderResourceParameters);

	FOUN_TRACE_GL_ERROR();
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

void DrawContext::ActivateSymbolBuffer(SymbolBuffer& symbolBuffer)
{
	m_pGraphicsDevice->ActivateVertex(symbolBuffer.m_pQuadVertexResource);

	m_pGraphicsDevice->ActivateIndex(symbolBuffer.m_pQuadIndexResource);
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

void DrawContext::DrawSymbolBuffer(SymbolBuffer& symbolBuffer)
{
	const grap::UniformResource* pModulateColorUniformResource = m_pUniformResourceArray + UNIFORM_MODULATE_COLOR;
	const grap::UniformResource* pTextureUniformResource = m_pUniformResourceArray + UNIFORM_TEXTURE;

	foun::FloatColor modulateColor = { 1.0f, 1.0f, 1.0f, 1.0f }; // TEMP!

	m_pGraphicsDevice->ActivateUniform(pModulateColorUniformResource, modulateColor);

	const imag::TextureResource* pTextureResource = symbolBuffer.m_pSheetAsset->pTextureAsset->pTextureResource;

	glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_INDEX);
	glBindTexture(GL_TEXTURE_2D, pTextureResource->hTexture);
	//glUniform1i(m_pTextureUniformResource->uniformLocation, TEXTURE_UNIT_INDEX);
	glUniform1i(pTextureUniformResource->uniformLocation, TEXTURE_UNIT_INDEX); // FINISH REFACTOR!

	FOUN_TRACE_GL_ERROR();

	u32 indexCount = symbolBuffer.m_quadCount * QUAD_INDEX_COUNT;
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, NULL);

	FOUN_TRACE_GL_ERROR();
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

//namespace font
}
//namespace lct
}
