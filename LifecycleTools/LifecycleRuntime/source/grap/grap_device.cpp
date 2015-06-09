#include <grap/grap_device.h>
#include <grap/grap_debug.h>

#include <foun/foun_debug.h>

namespace lct
{
namespace grap
{

/*
* Public Instance
*/
Device::Device()
: m_usedShaderResourceCount(0)
, m_usedVertexResourceCount(0)
, m_usedIndexResourceCount(0)
{
}

void Device::AcquireShaderResources(const ShaderResourceParameters& shaderResourceParameters)
{
	char log[1024];
	s32 logLength = 0;

	ShaderResource* pShaderResource = shaderResourceParameters.pShaderResource;
	if (pShaderResource->hShaderProgram != 0)
	{
		LCT_TRACE("Shader Resource already acquired!\n");
		return;
	}

	// create and compile the vertex shader
	pShaderResource->hVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(pShaderResource->hVertexShader, 1, (const GLchar**)&shaderResourceParameters.pVertexShaderBinary, 0);
	glCompileShader(pShaderResource->hVertexShader);
	LCT_TRACE_GL_ERROR();
	glGetShaderInfoLog(pShaderResource->hVertexShader, sizeof(log), &logLength, log);
	if (logLength > 0)
	{
		LCT_TRACE("Vertex Shader log:\n");
		LCT_TRACE("%s\n", log);
	}

	// create and compile the fragment shader
	pShaderResource->hFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(pShaderResource->hFragmentShader, 1, (const GLchar**)&shaderResourceParameters.pFragmentShaderBinary, 0);
	glCompileShader(pShaderResource->hFragmentShader);
	LCT_TRACE_GL_ERROR();
	glGetShaderInfoLog(pShaderResource->hFragmentShader, sizeof(log), &logLength, log);
	if (logLength > 0)
	{
		LCT_TRACE("Fragment Shader log:\n");
		LCT_TRACE("%s\n", log);
	}

	// create shader program and attach
	pShaderResource->hShaderProgram = glCreateProgram();
	glAttachShader(pShaderResource->hShaderProgram, pShaderResource->hVertexShader);
	glAttachShader(pShaderResource->hShaderProgram, pShaderResource->hFragmentShader);

	// bind attributes	
	for (u32 attributeIndex = 0; attributeIndex < shaderResourceParameters.attributeCount; ++attributeIndex)
	{
		const AttributeData* pAttributeData = shaderResourceParameters.pAttributeDataArray + attributeIndex;

		glBindAttribLocation(pShaderResource->hShaderProgram, attributeIndex, pAttributeData->name);
		LCT_TRACE_GL_ERROR();
	}

	// link shader program
	glLinkProgram(pShaderResource->hShaderProgram);
	LCT_TRACE_GL_ERROR();

	// find uniforms
	for (u32 uniformIndex = 0; uniformIndex < shaderResourceParameters.uniformCount; ++uniformIndex)
	{
		const UniformData* pUniformData = shaderResourceParameters.pUniformDataArray + uniformIndex;
		UniformResource* pUniformResource = shaderResourceParameters.pUniformResourceArray + uniformIndex;

		pUniformResource->uniformLocation = glGetUniformLocation(pShaderResource->hShaderProgram, pUniformData->name);
		LCT_TRACE_GL_ERROR();
	}

	++m_usedShaderResourceCount;
}

void Device::ReleaseShaderResources(const ShaderResourceParameters& shaderResourceParameters)
{
	ShaderResource* pShaderResource = shaderResourceParameters.pShaderResource;
	if (pShaderResource->hShaderProgram == 0)
	{
		LCT_TRACE("Shader Resource not yet acquired!\n");
		return;
	}

	// detatch and destroy shader program
	glDetachShader(pShaderResource->hShaderProgram, pShaderResource->hVertexShader);
	glDetachShader(pShaderResource->hShaderProgram, pShaderResource->hFragmentShader);
	glDeleteProgram(pShaderResource->hShaderProgram);

	// destroy vertex shader
	glDeleteShader(pShaderResource->hVertexShader);

	// destroy fragment shader
	glDeleteShader(pShaderResource->hFragmentShader);

	memset(pShaderResource, 0, sizeof(ShaderResource));

	// clear uniforms
	memset(shaderResourceParameters.pUniformResourceArray, 0, (sizeof(UniformResource) * shaderResourceParameters.uniformCount));

	LCT_TRACE_GL_ERROR();

	--m_usedShaderResourceCount;
}

void Device::AcquireVertexResource(const VertexResourceParameters& vertexResourceParameters)
{
	VertexResource* pVertexResource = vertexResourceParameters.pVertexResource;
	if (pVertexResource->hVertexBuffer != 0)
	{
		LCT_TRACE("Vertex Resource already acquired!\n");
		return;
	}
	GLenum usage = vertexResourceParameters.dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

	glGenBuffers(1, &pVertexResource->hVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, pVertexResource->hVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexResourceParameters.vertexBinarySize, vertexResourceParameters.pVertexBinary, usage);

	// calculate the stride for a single vertex
	u32 vertexStride = 0;
	for (u32 attributeIndex = 0; attributeIndex < vertexResourceParameters.attributeCount; ++attributeIndex)
	{
		const grap::AttributeData* pAttributeData = vertexResourceParameters.pAttributeDataArray + attributeIndex;

		u32 attributeSize = pAttributeData->componentCount * sizeof(f32);
		vertexStride += attributeSize;
	}
	pVertexResource->vertexStride = vertexStride;

	// store references to attribute information
	pVertexResource->pAttributeDataArray = vertexResourceParameters.pAttributeDataArray;
	pVertexResource->attributeCount = vertexResourceParameters.attributeCount;

	LCT_TRACE_GL_ERROR();

	++m_usedVertexResourceCount;
}

void Device::RefreshVertexResource(const VertexResourceParameters& vertexResourceParameters)
{
	VertexResource* pVertexResource = vertexResourceParameters.pVertexResource;
	if (pVertexResource->hVertexBuffer == 0)
	{
		LCT_TRACE("Vertex Resource not yet acquired!\n");
		return;
	}

	glBindBuffer(GL_ARRAY_BUFFER, pVertexResource->hVertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertexResourceParameters.vertexBinarySize, vertexResourceParameters.pVertexBinary);

	LCT_TRACE_GL_ERROR();
}

void Device::ReleaseVertexResource(const VertexResourceParameters& vertexResourceParameters)
{
	VertexResource* pVertexResource = vertexResourceParameters.pVertexResource;
	if (pVertexResource->hVertexBuffer == 0)
	{
		LCT_TRACE("Vertex Resource not yet acquired!\n");
		return;
	}

	glDeleteBuffers(1, &pVertexResource->hVertexBuffer);

	pVertexResource->hVertexBuffer = 0;

	// clear references to attribute information
	pVertexResource->pAttributeDataArray = NULL;
	pVertexResource->attributeCount = 0;

	LCT_TRACE_GL_ERROR();

	--m_usedVertexResourceCount;
}

void Device::AcquireIndexResource(const IndexResourceParameters& indexResourceParameters)
{
	IndexResource* pIndexResource = indexResourceParameters.pIndexResource;
	if (pIndexResource->hIndexBuffer != 0)
	{
		LCT_TRACE("Index Resource already acquired!\n");
		return;
	}
	GLenum usage = indexResourceParameters.dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

	glGenBuffers(1, &pIndexResource->hIndexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, pIndexResource->hIndexBuffer);
	glBufferData(GL_ARRAY_BUFFER, indexResourceParameters.indexBinarySize, indexResourceParameters.pIndexBinary, usage);

	LCT_TRACE_GL_ERROR();

	++m_usedIndexResourceCount;
}

void Device::RefreshIndexResource(const IndexResourceParameters& indexResourceParameters)
{
	IndexResource* pIndexResource = indexResourceParameters.pIndexResource;
	if (pIndexResource->hIndexBuffer == 0)
	{
		LCT_TRACE("Index Resource not yet acquired!\n");
		return;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pIndexResource->hIndexBuffer);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indexResourceParameters.indexBinarySize, indexResourceParameters.pIndexBinary);

	LCT_TRACE_GL_ERROR();
}

void Device::ReleaseIndexResource(const IndexResourceParameters& indexResourceParameters)
{
	IndexResource* pIndexResource = indexResourceParameters.pIndexResource;
	if (pIndexResource->hIndexBuffer == 0)
	{
		LCT_TRACE("Index Resource not yet acquired!\n");
		return;
	}

	glDeleteBuffers(1, &pIndexResource->hIndexBuffer);

	pIndexResource->hIndexBuffer = 0;

	LCT_TRACE_GL_ERROR();

	--m_usedIndexResourceCount;
}

void Device::ActivateShader(const ShaderResource* pShaderResource)
{
	glUseProgram(pShaderResource->hShaderProgram);

	LCT_TRACE_GL_ERROR();
}

void Device::ActivateUniform(const UniformResource* pUniformResource, const foun::Vector4& vector)
{
	glUniform4fv(pUniformResource->uniformLocation, 1, (GLfloat*)&vector);

	LCT_TRACE_GL_ERROR();
}

void Device::ActivateUniform(const UniformResource* pUniformResource, const foun::FloatColor& color)
{
	glUniform4fv(pUniformResource->uniformLocation, 1, (GLfloat*)&color);

	LCT_TRACE_GL_ERROR();
}

void Device::ActivateUniform(const UniformResource* pUniformResource, const foun::Matrix33& matrix)
{
	glUniformMatrix3fv(pUniformResource->uniformLocation, 1, false, (GLfloat*)&matrix);

	LCT_TRACE_GL_ERROR();
}

void Device::ActivateUniform(const UniformResource* pUniformResource, const foun::Matrix44& matrix)
{
	glUniformMatrix4fv(pUniformResource->uniformLocation, 1, false, (GLfloat*)&matrix);

	LCT_TRACE_GL_ERROR();
}

void Device::ActivateVertex(const VertexResource* pVertexResource)
{
	glBindBuffer(GL_ARRAY_BUFFER, pVertexResource->hVertexBuffer);

	GLuint attributeOffset = 0;
	for (u32 attributeIndex = 0; attributeIndex < pVertexResource->attributeCount; ++attributeIndex)
	{
		const grap::AttributeData* pAttributeData = pVertexResource->pAttributeDataArray + attributeIndex;
		glVertexAttribPointer(attributeIndex, pAttributeData->componentCount, GL_FLOAT, GL_FALSE, pVertexResource->vertexStride, (GLvoid*)attributeOffset);
		glEnableVertexAttribArray(attributeIndex);

		u32 attributeSize = pAttributeData->componentCount * sizeof(f32);
		attributeOffset += attributeSize;
	}

	LCT_TRACE_GL_ERROR();
}

void Device::ActivateIndex(const IndexResource* pIndexResource)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pIndexResource->hIndexBuffer);

	LCT_TRACE_GL_ERROR();
}

/*
* Protected Instance
*/


//namespace grap
}
//namespace lct
}
