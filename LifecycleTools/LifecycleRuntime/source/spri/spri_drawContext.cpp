#include <spri/spri_drawContext.h>
#include <spri/spri_data.h>
#include <spri/spri_figureInstance.h>
#include <spri/spri_constants.h>
#include <spri/spri_arrays.h>

#include <grap/grap_device.h>
#include <grap/grap_data.h>
#include <grap/grap_resources.h>
#include <grap/grap_parameters.h>

#include <imag/imag_resources.h>

#include <util/util_indexMap.h>

#include <foun/foun_debug.h>
#include <foun/foun_graphicsDebug.h>
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
		foun::FloatColor& modulateColor = (foun::FloatColor&)part.propertyArray[PART_PROPERTY_TYPE_MODULATE_COLOR_R];
		foun::FloatColor& interpolateColor = (foun::FloatColor&)part.propertyArray[PART_PROPERTY_TYPE_INTERPOLATE_COLOR_R];		

		m_pGraphicsDevice->ActivateUniform(pJointTransformUniformResource, tempJointMatrix);
		m_pGraphicsDevice->ActivateUniform(pPlacementUniformResource, part.placement);
		m_pGraphicsDevice->ActivateUniform(pModulateColorUniformResource, modulateColor);
		m_pGraphicsDevice->ActivateUniform(pInterpolateColorUniformResource, interpolateColor);

		glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_INDEX);
		glBindTexture(GL_TEXTURE_2D, pattern.pTextureResource->hTexture);
		//glUniform1i(m_pTextureUniformResource->uniformLocation, TEXTURE_UNIT_INDEX);
		glUniform1i(pTextureUniformResource->uniformLocation, TEXTURE_UNIT_INDEX); // FINISH REFACTOR!

		FOUN_TRACE_GL_ERROR();

		glDrawElements(GL_TRIANGLES, QUAD_INDEX_COUNT, GL_UNSIGNED_BYTE, NULL);

		FOUN_TRACE_GL_ERROR();
	}
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
