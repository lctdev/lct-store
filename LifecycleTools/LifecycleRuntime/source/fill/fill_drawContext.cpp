#include <fill/fill_drawContext.h>
#include <fill/fill_data.h>
#include <fill/fill_resources.h>

#include <shad/shad_resourceHandler.h>
#include <shad/shad_data.h>
#include <shad/shad_resources.h>

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
static const u32 ATTRIBUTE_COUNT = 1;
static shad::AttributeData ATTRIBUTE_DATA_ARRAY[ATTRIBUTE_COUNT] =
{
	{ "attr_position", 3 }
};

static const u32 QUAD_VERTEX_COUNT = 4;
static VertexData QUAD_VERTEX_DATA_ARRAY[QUAD_VERTEX_COUNT] =
{
	{ -0.5f, -0.5f, 0.0f },
	{ 0.5f, -0.5f, 0.0f },
	{ 0.5f, 0.5f, 0.0f },
	{ -0.5f, 0.5f, 0.0f }
};

static const u32 QUAD_INDEX_COUNT = 6;
static IndexData QUAD_INDEX_DATA_ARRAY[QUAD_INDEX_COUNT] =
{
	0,
	1,
	2,
	2,
	3,
	0
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
, m_pPlacementUniformResource(NULL)
, m_pProjectionTransformUniformResource(NULL)
, m_pWorldTransformUniformResource(NULL)
, m_pModulateColorUniformResource(NULL)
, m_pQuadResource(NULL)
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

	m_pPlacementUniformResource = m_pAllocator->AllocType<shad::UniformResource>();
	m_pProjectionTransformUniformResource = m_pAllocator->AllocType<shad::UniformResource>();
	m_pWorldTransformUniformResource = m_pAllocator->AllocType<shad::UniformResource>();
	m_pModulateColorUniformResource = m_pAllocator->AllocType<shad::UniformResource>();

	m_pQuadResource = m_pAllocator->AllocType<QuadResource>();
}

void DrawContext::AcquireResources()
{
	LCT_TRACE("fill::DrawContext::Acquireresources\n");

	shad::ShaderData shaderData;
	shaderData.vertexShaderSize = 0; // TEMP?
	shaderData.fragmentShaderSize = 0;
	shaderData.attributeCount = ATTRIBUTE_COUNT;
	m_pShaderResourceHandler->AcquireShaderResources(m_pShaderResource, &shaderData, m_pVertexShaderBinary, m_pFragmentShaderBinary,
			m_pAttributeResourceArray, ATTRIBUTE_DATA_ARRAY);

	AcquireUniformResource(m_pPlacementUniformResource, "unif_placement");
	AcquireUniformResource(m_pProjectionTransformUniformResource, "unif_projectionTransform");
	AcquireUniformResource(m_pWorldTransformUniformResource, "unif_worldTransform");
	AcquireUniformResource(m_pModulateColorUniformResource, "unif_modulateColor");

	glGenBuffers(1, &m_pQuadResource->hVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_pQuadResource->hVertexBuffer);
	u32 vertexDataSize = sizeof(QUAD_VERTEX_DATA_ARRAY);
	glBufferData(GL_ARRAY_BUFFER, vertexDataSize, QUAD_VERTEX_DATA_ARRAY, GL_STATIC_DRAW);

	glGenBuffers(1, &m_pQuadResource->hIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pQuadResource->hIndexBuffer);
	u32 indexDataSize = sizeof(QUAD_INDEX_DATA_ARRAY);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSize, QUAD_INDEX_DATA_ARRAY, GL_STATIC_DRAW);

	FOUN_TRACE_GL_ERROR();
}

void DrawContext::ReleaseResources()
{
	shad::ShaderData shaderData;
	shaderData.vertexShaderSize = 0; // TEMP?
	shaderData.fragmentShaderSize = 0;
	shaderData.attributeCount = ATTRIBUTE_COUNT;
	m_pShaderResourceHandler->ReleaseShaderResources(m_pShaderResource, &shaderData, m_pAttributeResourceArray);

	m_pShaderResourceHandler->ReleaseUniformResource(m_pPlacementUniformResource);
	m_pShaderResourceHandler->ReleaseUniformResource(m_pProjectionTransformUniformResource);
	m_pShaderResourceHandler->ReleaseUniformResource(m_pWorldTransformUniformResource);
	m_pShaderResourceHandler->ReleaseUniformResource(m_pModulateColorUniformResource);

	glDeleteBuffers(1, &m_pQuadResource->hVertexBuffer);

	m_pQuadResource->hVertexBuffer = 0;

	glDeleteBuffers(1, &m_pQuadResource->hIndexBuffer);

	m_pQuadResource->hIndexBuffer = 0;

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

void DrawContext::ActivateQuad()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_pQuadResource->hVertexBuffer);
	FOUN_TRACE_GL_ERROR();

	for (u32 attributeIndex = 0; attributeIndex < ATTRIBUTE_COUNT; ++attributeIndex)
	{
		const shad::AttributeData* pAttributeData = ATTRIBUTE_DATA_ARRAY + attributeIndex;
		shad::AttributeResource* pAttributeResource = m_pAttributeResourceArray + attributeIndex;
		glVertexAttribPointer(attributeIndex, pAttributeData->componentCount, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)pAttributeResource->offset);
		FOUN_TRACE_GL_ERROR();
		glEnableVertexAttribArray(attributeIndex);
		FOUN_TRACE_GL_ERROR();
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pQuadResource->hIndexBuffer);
	FOUN_TRACE_GL_ERROR();

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

void DrawContext::DrawRect(const foun::RectCentered& rect, const foun::FloatColor& color)
{
	foun::Vector4 placement;
	placement.x = rect.width;
	placement.y = rect.height;
	placement.z = rect.x;
	placement.w = rect.y;
	glUniform4fv(m_pPlacementUniformResource->uniformLocation, 1, (GLfloat*)&placement);

	glUniform4fv(m_pModulateColorUniformResource->uniformLocation, 1, (GLfloat*)&color);

	glDrawElements(GL_TRIANGLES, QUAD_INDEX_COUNT, GL_UNSIGNED_BYTE, NULL);
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

//namespace spri
}
//namespace lct
}
