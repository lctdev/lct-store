#include <spri/spri_drawContext.h>
#include <spri/spri_data.h>
#include <spri/spri_figureInstance.h>
#include <spri/spri_constants.h>
#include <spri/spri_arrays.h>

#include <grap/grap_device.h>
#include <grap/grap_data.h>
#include <grap/grap_resources.h>
#include <grap/grap_parameters.h>

#include <util/util_indexMap.h>

#include <foun/foun_debug.h>
#include <foun/foun_string.h>

namespace lct
{
namespace spri
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
	LCT_TRACE("spri::DrawContext::Acquireresources\n");

	grap::ShaderSetupParameters shaderSetupParameters;
	FillShaderSetupParameters(shaderSetupParameters);
	m_pGraphicsDevice->AcquireShaderResources(shaderSetupParameters);

	grap::VertexSetupParameters vertexSetupParameters;
	grap::IndexSetupParameters indexSetupParameters;
	FillQuadSetupParameters(vertexSetupParameters, indexSetupParameters);
	m_pGraphicsDevice->AcquireVertexResource(vertexSetupParameters);
	m_pGraphicsDevice->AcquireIndexResource(indexSetupParameters);
}

void DrawContext::ReleaseResources()
{
	grap::ShaderSetupParameters shaderSetupParameters;
	FillShaderSetupParameters(shaderSetupParameters);
	m_pGraphicsDevice->ReleaseShaderResources(shaderSetupParameters);

	grap::VertexSetupParameters vertexSetupParameters;
	grap::IndexSetupParameters indexSetupParameters;
	FillQuadSetupParameters(vertexSetupParameters, indexSetupParameters);
	m_pGraphicsDevice->ReleaseVertexResource(vertexSetupParameters);
	m_pGraphicsDevice->ReleaseIndexResource(indexSetupParameters);
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

void DrawContext::ActivateQuad()
{
	m_pGraphicsDevice->ActivateVertex(m_pQuadVertexResource);

	m_pGraphicsDevice->ActivateIndex(m_pQuadIndexResource);
}

void DrawContext::DeactivateQuad()
{
	m_pGraphicsDevice->DeactivateVertex(m_pQuadVertexResource);
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

void DrawContext::DrawFigure(FigureInstance& figureInstance)
{
	foun::Matrix33 tempJointMatrix;
	FigureInstance::Part* pPartArray = figureInstance.m_pPartArray;
	FigureInstance::Joint* pJointArray = figureInstance.m_pJointArray;
	FigureInstance::Pattern* pPatternArray = figureInstance.m_pPatternArray;
	const grap::UniformResource* pJointTransformUniformResource = m_pUniformResourceArray + UNIFORM_JOINT_TRANSFORM;
	const grap::UniformResource* pPlacementUniformResource = m_pUniformResourceArray + UNIFORM_PLACEMENT;
	const grap::UniformResource* pModulateColorUniformResource = m_pUniformResourceArray + UNIFORM_MODULATE_COLOR;
	const grap::UniformResource* pInterpolateColorUniformResource = m_pUniformResourceArray + UNIFORM_INTERPOLATE_COLOR;
	const grap::UniformResource* pTextureUniformResource = m_pUniformResourceArray + UNIFORM_TEXTURE;

	for (u32 partIndex = 0; partIndex < figureInstance.m_partCount; ++partIndex)
	{
		FigureInstance::Part& part = pPartArray[partIndex];
		FigureInstance::Joint& joint = pJointArray[part.pPartData->jointIndex];
		FigureInstance::Pattern& pattern = pPatternArray[part.pPartData->patternIndex];

		foun::Matrix33FromMatrix32(tempJointMatrix, joint.transform);
		foun::FloatColor4& modulateColor = (foun::FloatColor4&)part.propertyArray[PART_PROPERTY_TYPE_MODULATE_COLOR_R];
		foun::FloatColor4& interpolateColor = (foun::FloatColor4&)part.propertyArray[PART_PROPERTY_TYPE_INTERPOLATE_COLOR_R];		

		m_pGraphicsDevice->ActivateUniform(pJointTransformUniformResource, tempJointMatrix);
		m_pGraphicsDevice->ActivateUniform(pPlacementUniformResource, part.placement);
		m_pGraphicsDevice->ActivateUniform(pModulateColorUniformResource, modulateColor);
		m_pGraphicsDevice->ActivateUniform(pInterpolateColorUniformResource, interpolateColor);

		grap::TextureBindParameters textureBindParameters;
		textureBindParameters.pTextureResource = pattern.pTextureResource;
		textureBindParameters.pUniformResource = pTextureUniformResource;
		textureBindParameters.textureUnitIndex = TEXTURE_UNIT_INDEX;
		m_pGraphicsDevice->ActivateTexture(textureBindParameters);

		m_pGraphicsDevice->Draw(QUAD_INDEX_COUNT, 0, grap::INDEX_TYPE_U8);
	}
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

void DrawContext::FillQuadSetupParameters(grap::VertexSetupParameters& vertexSetupParameters, grap::IndexSetupParameters& indexSetupParameters)
{
	vertexSetupParameters.pVertexResource = m_pQuadVertexResource;
	vertexSetupParameters.pVertexBinary = QUAD_VERTEX_DATA_ARRAY;
	vertexSetupParameters.vertexBinarySize = sizeof(QUAD_VERTEX_DATA_ARRAY);
	vertexSetupParameters.pAttributeDataArray = ATTRIBUTE_DATA_ARRAY;
	vertexSetupParameters.attributeCount = ATTRIBUTE_COUNT;
	vertexSetupParameters.dynamic = false;

	indexSetupParameters.pIndexResource = m_pQuadIndexResource;
	indexSetupParameters.pIndexBinary = QUAD_INDEX_DATA_ARRAY;
	indexSetupParameters.indexBinarySize = sizeof(QUAD_INDEX_DATA_ARRAY);
	indexSetupParameters.dynamic = false;
}

//namespace spri
}
//namespace lct
}
