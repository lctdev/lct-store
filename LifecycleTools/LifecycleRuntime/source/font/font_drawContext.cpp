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

#include <util/util_indexMap.h>

#include <foun/foun_debug.h>
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
	renderStateParameters.blendType = grap::BLEND_TYPE_MULTIPLICATIVE;
	m_pGraphicsDevice->ActivateRenderState(renderStateParameters);
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

void DrawContext::DeactivateSymbolBuffer(SymbolBuffer& symbolBuffer)
{
	m_pGraphicsDevice->DeactivateVertex(symbolBuffer.m_pQuadVertexResource);
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

	foun::FloatColor4 modulateColor = { 1.0f, 1.0f, 1.0f, 1.0f }; // TEMP!

	m_pGraphicsDevice->ActivateUniform(pModulateColorUniformResource, modulateColor);

	const grap::TextureResource* pTextureResource = symbolBuffer.m_pSheetAsset->pTextureAsset->pTextureResource;

	grap::TextureBindParameters textureBindParameters;
	textureBindParameters.pTextureResource = pTextureResource;
	textureBindParameters.pUniformResource = pTextureUniformResource;
	textureBindParameters.textureUnitIndex = TEXTURE_UNIT_INDEX;
	m_pGraphicsDevice->ActivateTexture(textureBindParameters);

	u32 indexCount = symbolBuffer.m_quadCount * QUAD_INDEX_COUNT;
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

//namespace font
}
//namespace lct
}
