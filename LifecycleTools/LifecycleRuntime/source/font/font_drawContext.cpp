#include <font/font_drawContext.h>
#include <font/font_assets.h>
#include <font/font_data.h>
#include <font/font_resources.h>
#include <font/font_symbolBuffer.h>
#include <font/font_constants.h>

#include <shad/shad_resourceHandler.h>
#include <shad/shad_data.h>
#include <shad/shad_resources.h>

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
static const u32 ATTRIBUTE_COUNT = 3;
static shad::AttributeData ATTRIBUTE_DATA_ARRAY[ATTRIBUTE_COUNT] =
{
	{ "attr_position", 3 },
	{ "attr_texCoord", 2 },
	{ "attr_color", 4 }
};

/*
 * Public Instance
 */
DrawContext::DrawContext()
: m_pAllocator(NULL)
, m_pShaderResourceHandler(NULL)
, m_pVertexShaderBinary(NULL)
, m_pFragmentShaderBinary(NULL)
, m_pShaderResource(NULL)
, m_pAttributeResourceArray(NULL)
, m_pProjectionTransformUniformResource(NULL)
, m_pWorldTransformUniformResource(NULL)
, m_pModulateColorUniformResource(NULL)
, m_pTextureUniformResource(NULL)
{
}

void DrawContext::SetAllocator(lct::foun::Allocator* pAllocator)
{
	m_pAllocator = pAllocator;
}

void DrawContext::SetShaderResourceHandler(shad::ResourceHandler* pShaderResourceHandler)
{
	m_pShaderResourceHandler = pShaderResourceHandler;
}

void DrawContext::SetShaderBinaries(void* pVertexShaderBinary, void* pFragmentShaderBinary)
{
	m_pVertexShaderBinary =  pVertexShaderBinary;
	m_pFragmentShaderBinary = pFragmentShaderBinary;
}

void DrawContext::CreateResources()
{
	m_pShaderResource = m_pAllocator->AllocType<shad::ShaderResource>();

	m_pAttributeResourceArray = m_pAllocator->AllocTypeArray<shad::AttributeResource>(ATTRIBUTE_COUNT);

	m_pProjectionTransformUniformResource = m_pAllocator->AllocType<shad::UniformResource>();
	m_pWorldTransformUniformResource = m_pAllocator->AllocType<shad::UniformResource>();
	m_pModulateColorUniformResource = m_pAllocator->AllocType<shad::UniformResource>();
	m_pTextureUniformResource = m_pAllocator->AllocType<shad::UniformResource>();
}

void DrawContext::AcquireResources()
{
	LCT_TRACE("font::DrawContext::Acquireresources\n");

	shad::ShaderData shaderData;
	shaderData.vertexShaderSize = 0; // TEMP?
	shaderData.fragmentShaderSize = 0;
	shaderData.attributeCount = ATTRIBUTE_COUNT;
	m_pShaderResourceHandler->AcquireShaderResources(m_pShaderResource, &shaderData, m_pVertexShaderBinary, m_pFragmentShaderBinary,
			m_pAttributeResourceArray, ATTRIBUTE_DATA_ARRAY);

	AcquireUniformResource(m_pProjectionTransformUniformResource, "unif_projectionTransform");
	AcquireUniformResource(m_pWorldTransformUniformResource, "unif_worldTransform");
	AcquireUniformResource(m_pModulateColorUniformResource, "unif_modulateColor");
	AcquireUniformResource(m_pTextureUniformResource, "unif_texture");

	FOUN_TRACE_GL_ERROR();
}

void DrawContext::ReleaseResources()
{
	shad::ShaderData shaderData;
	shaderData.vertexShaderSize = 0; // TEMP?
	shaderData.fragmentShaderSize = 0;
	shaderData.attributeCount = ATTRIBUTE_COUNT;
	m_pShaderResourceHandler->ReleaseShaderResources(m_pShaderResource, &shaderData, m_pAttributeResourceArray);

	m_pShaderResourceHandler->ReleaseUniformResource(m_pProjectionTransformUniformResource);
	m_pShaderResourceHandler->ReleaseUniformResource(m_pWorldTransformUniformResource);
	m_pShaderResourceHandler->ReleaseUniformResource(m_pModulateColorUniformResource);
	m_pShaderResourceHandler->ReleaseUniformResource(m_pTextureUniformResource);

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
	glUseProgram(m_pShaderResource->hShaderProgram);

	FOUN_TRACE_GL_ERROR();
}

void DrawContext::ActivateSymbolBuffer(SymbolBuffer& symbolBuffer)
{
	glBindBuffer(GL_ARRAY_BUFFER, symbolBuffer.m_pQuadResource->hVertexBuffer);

	for (u32 attributeIndex = 0; attributeIndex < ATTRIBUTE_COUNT; ++attributeIndex)
	{
		const shad::AttributeData* pAttributeData = ATTRIBUTE_DATA_ARRAY + attributeIndex;
		shad::AttributeResource* pAttributeResource = m_pAttributeResourceArray + attributeIndex;
		glVertexAttribPointer(attributeIndex, pAttributeData->componentCount, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)pAttributeResource->offset);
		glEnableVertexAttribArray(attributeIndex);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, symbolBuffer.m_pQuadResource->hIndexBuffer);

	FOUN_TRACE_GL_ERROR();
}

void DrawContext::ActivateProjectionTransform(const foun::Matrix44& projectionTransform)
{
	glUniformMatrix4fv(m_pProjectionTransformUniformResource->uniformLocation, 1, false, (GLfloat*)&projectionTransform);

	FOUN_TRACE_GL_ERROR();
}

void DrawContext::ActivateWorldTransform(const foun::Matrix32& worldTransform)
{
	foun::Matrix33 tempWorldMatrix;
	foun::Matrix33FromMatrix32(tempWorldMatrix, worldTransform);
	glUniformMatrix3fv(m_pWorldTransformUniformResource->uniformLocation, 1, false, (GLfloat*)&tempWorldMatrix);

	FOUN_TRACE_GL_ERROR();
}

void DrawContext::DrawSymbolBuffer(SymbolBuffer& symbolBuffer)
{
	f32 modulateColor[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // TEMP!

	glUniform4fv(m_pModulateColorUniformResource->uniformLocation, 1, (GLfloat*)modulateColor);

	FOUN_TRACE_GL_ERROR();

	const imag::TextureResource* pTextureResource = symbolBuffer.m_pSheetAsset->pTextureAsset->pTextureResource;
	glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_INDEX);
	glBindTexture(GL_TEXTURE_2D, pTextureResource->hTexture);
	glUniform1i(m_pTextureUniformResource->uniformLocation, TEXTURE_UNIT_INDEX);

	FOUN_TRACE_GL_ERROR();

	u32 indexCount = symbolBuffer.m_quadCount * QUAD_INDEX_COUNT;
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, NULL);

	FOUN_TRACE_GL_ERROR();
}

/*
 * Protected Instance
 */
void DrawContext::AcquireUniformResource(shad::UniformResource* pUniformResource, const char* pName)
{
	shad::UniformData uniformData;
	LCT_STRCPY(uniformData.name, sizeof(uniformData.name), pName);
	m_pShaderResourceHandler->AcquireUniformResource(m_pShaderResource, pUniformResource, &uniformData);
}

//namespace font
}
//namespace lct
}
