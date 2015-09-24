#include <grap/grap_device.h>
#include <grap/grap_debug.h>

#include <foun/foun_debug.h>

#include <string.h>

namespace lct
{
namespace grap
{

/*
* Public Instance
*/
Device::Device()
: m_acquiredShaderResourceCount(0)
, m_acquiredVertexResourceCount(0)
, m_acquiredIndexResourceCount(0)
, m_acquiredTextureResourceCount(0)
{
}

void Device::AcquireShaderResources(const ShaderSetupParameters& shaderSetupParameters)
{
	char log[1024];
	s32 logLength = 0;

	ShaderResource* pShaderResource = shaderSetupParameters.pShaderResource;
	if (pShaderResource->hShaderProgram != 0)
	{
		LCT_TRACE("[Device::AcquireShaderResources] Shader Resource already acquired!\n");
		return;
	}

	// create and compile the vertex shader
	pShaderResource->hVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(pShaderResource->hVertexShader, 1, (const GLchar**)&shaderSetupParameters.pVertexShaderBinary, 0);
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
	glShaderSource(pShaderResource->hFragmentShader, 1, (const GLchar**)&shaderSetupParameters.pFragmentShaderBinary, 0);
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
	for (u32 attributeIndex = 0; attributeIndex < shaderSetupParameters.attributeCount; ++attributeIndex)
	{
		const AttributeData* pAttributeData = shaderSetupParameters.pAttributeDataArray + attributeIndex;

		glBindAttribLocation(pShaderResource->hShaderProgram, attributeIndex, pAttributeData->name);
		LCT_TRACE_GL_ERROR();
	}

	// link shader program
	glLinkProgram(pShaderResource->hShaderProgram);
	LCT_TRACE_GL_ERROR();

	// find uniforms
	for (u32 uniformIndex = 0; uniformIndex < shaderSetupParameters.uniformCount; ++uniformIndex)
	{
		const UniformData* pUniformData = shaderSetupParameters.pUniformDataArray + uniformIndex;
		UniformResource* pUniformResource = shaderSetupParameters.pUniformResourceArray + uniformIndex;

		pUniformResource->uniformLocation = glGetUniformLocation(pShaderResource->hShaderProgram, pUniformData->name);
		LCT_TRACE_GL_ERROR();
	}

	++m_acquiredShaderResourceCount;
}

void Device::ReleaseShaderResources(const ShaderSetupParameters& shaderSetupParameters)
{
	ShaderResource* pShaderResource = shaderSetupParameters.pShaderResource;
	if (pShaderResource->hShaderProgram == 0)
	{
		LCT_TRACE("[Device::ReleaseShaderResources] Shader Resource not yet acquired!\n");
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
	memset(shaderSetupParameters.pUniformResourceArray, 0, (sizeof(UniformResource) * shaderSetupParameters.uniformCount));

	LCT_TRACE_GL_ERROR();

	--m_acquiredShaderResourceCount;
}

void Device::AcquireVertexResource(const VertexSetupParameters& vertexSetupParameters)
{
	VertexResource* pVertexResource = vertexSetupParameters.pVertexResource;
	if (pVertexResource->hVertexBuffer != 0)
	{
		LCT_TRACE("[Device::AcquireVertexResource] Vertex Resource already acquired!\n");
		return;
	}
	GLenum usage = vertexSetupParameters.dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

	glGenBuffers(1, &pVertexResource->hVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, pVertexResource->hVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexSetupParameters.vertexBinarySize, vertexSetupParameters.pVertexBinary, usage);

	// calculate the stride for a single vertex
	u32 vertexStride = 0;
	for (u32 attributeIndex = 0; attributeIndex < vertexSetupParameters.attributeCount; ++attributeIndex)
	{
		const grap::AttributeData* pAttributeData = vertexSetupParameters.pAttributeDataArray + attributeIndex;

		u32 attributeSize = pAttributeData->componentCount * sizeof(f32);
		vertexStride += attributeSize;
	}
	pVertexResource->vertexStride = vertexStride;

	// store references to attribute information
	pVertexResource->pAttributeDataArray = vertexSetupParameters.pAttributeDataArray;
	pVertexResource->attributeCount = vertexSetupParameters.attributeCount;

	LCT_TRACE_GL_ERROR();

	++m_acquiredVertexResourceCount;
}

void Device::RefreshVertexResource(const VertexSetupParameters& vertexSetupParameters)
{
	VertexResource* pVertexResource = vertexSetupParameters.pVertexResource;
	if (pVertexResource->hVertexBuffer == 0)
	{
		LCT_TRACE("[Device::RefreshVertexResource] Vertex Resource not yet acquired!\n");
		return;
	}

	glBindBuffer(GL_ARRAY_BUFFER, pVertexResource->hVertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSetupParameters.vertexBinarySize, vertexSetupParameters.pVertexBinary);

	LCT_TRACE_GL_ERROR();
}

void Device::ReleaseVertexResource(const VertexSetupParameters& vertexSetupParameters)
{
	VertexResource* pVertexResource = vertexSetupParameters.pVertexResource;
	if (pVertexResource->hVertexBuffer == 0)
	{
		LCT_TRACE("[Device::ReleaseVertexResource] Vertex Resource not yet acquired!\n");
		return;
	}

	glDeleteBuffers(1, &pVertexResource->hVertexBuffer);

	pVertexResource->hVertexBuffer = 0;

	// clear references to attribute information
	pVertexResource->pAttributeDataArray = NULL;
	pVertexResource->attributeCount = 0;

	LCT_TRACE_GL_ERROR();

	--m_acquiredVertexResourceCount;
}

void Device::AcquireIndexResource(const IndexSetupParameters& indexSetupParameters)
{
	IndexResource* pIndexResource = indexSetupParameters.pIndexResource;
	if (pIndexResource->hIndexBuffer != 0)
	{
		LCT_TRACE("[Device::AcquireIndexResource] Index Resource already acquired!\n");
		return;
	}
	GLenum usage = indexSetupParameters.dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

	glGenBuffers(1, &pIndexResource->hIndexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, pIndexResource->hIndexBuffer);
	glBufferData(GL_ARRAY_BUFFER, indexSetupParameters.indexBinarySize, indexSetupParameters.pIndexBinary, usage);

	LCT_TRACE_GL_ERROR();

	++m_acquiredIndexResourceCount;
}

void Device::RefreshIndexResource(const IndexSetupParameters& indexSetupParameters)
{
	IndexResource* pIndexResource = indexSetupParameters.pIndexResource;
	if (pIndexResource->hIndexBuffer == 0)
	{
		LCT_TRACE("[Device::RefreshIndexResource] Index Resource not yet acquired!\n");
		return;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pIndexResource->hIndexBuffer);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indexSetupParameters.indexBinarySize, indexSetupParameters.pIndexBinary);

	LCT_TRACE_GL_ERROR();
}

void Device::ReleaseIndexResource(const IndexSetupParameters& indexSetupParameters)
{
	IndexResource* pIndexResource = indexSetupParameters.pIndexResource;
	if (pIndexResource->hIndexBuffer == 0)
	{
		LCT_TRACE("[Device::ReleaseIndexResource] Index Resource not yet acquired!\n");
		return;
	}

	glDeleteBuffers(1, &pIndexResource->hIndexBuffer);

	pIndexResource->hIndexBuffer = 0;

	LCT_TRACE_GL_ERROR();

	--m_acquiredIndexResourceCount;
}

void Device::AcquireTextureResource(const TextureSetupParameters& textureSetupParameters)
{
	TextureResource* pTextureResource = textureSetupParameters.pTextureResource;
	if (pTextureResource->hTexture != 0)
	{
		LCT_TRACE("[Device::AcquireTextureResource] Texture Resource already acquired!\n");
		return;
	}

	// TEMP?
	const GLint INTERNAL_FORMAT = GL_RGBA;
	const GLint FORMAT = GL_RGBA;
	const GLint TYPE = GL_UNSIGNED_BYTE;

	glGenTextures(1, &pTextureResource->hTexture);
	glBindTexture(GL_TEXTURE_2D, pTextureResource->hTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, INTERNAL_FORMAT, textureSetupParameters.width, textureSetupParameters.height, 0, FORMAT, TYPE, textureSetupParameters.pTextureBinary);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	LCT_TRACE_GL_ERROR();

	++m_acquiredTextureResourceCount;
}

void Device::ReleaseTextureResource(const TextureSetupParameters& textureSetupParameters)
{
	TextureResource* pTextureResource = textureSetupParameters.pTextureResource;
	if (pTextureResource->hTexture == 0)
	{
		LCT_TRACE("[Device::ReleaseTextureResource] Texture Resource not yet acquired!\n");
		return;
	}

	glDeleteTextures(1, &pTextureResource->hTexture);

	pTextureResource->hTexture = 0;

	LCT_TRACE_GL_ERROR();

	--m_acquiredTextureResourceCount;
}

void Device::ActivateRenderState(RenderStateParameters& renderStateParameters)
{
	if (renderStateParameters.enableBlend)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glDisable(GL_BLEND);
	}

	LCT_TRACE_GL_ERROR();
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

void Device::DeactivateVertex(const VertexResource* pVertexResource)
{
	for (u32 attributeIndex = 0; attributeIndex < pVertexResource->attributeCount; ++attributeIndex)
	{
		glDisableVertexAttribArray(attributeIndex);
	}

	LCT_TRACE_GL_ERROR();
}

void Device::ActivateIndex(const IndexResource* pIndexResource)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pIndexResource->hIndexBuffer);

	LCT_TRACE_GL_ERROR();
}

void Device::ActivateTexture(TextureBindParameters& textureBindParameters)
{
	const TextureResource* pTextureResource = textureBindParameters.pTextureResource;
	const UniformResource* pUniformResource = textureBindParameters.pUniformResource;

	glActiveTexture(GL_TEXTURE0 + textureBindParameters.textureUnitIndex);
	glBindTexture(GL_TEXTURE_2D, pTextureResource->hTexture);
	glUniform1i(pUniformResource->uniformLocation, textureBindParameters.textureUnitIndex);

	LCT_TRACE_GL_ERROR();
}

void Device::Draw(u32 indexCount, u32 indexOffset, IndexType indexType)
{
	static const GLenum TYPE_TABLE[INDEX_TYPE_COUNT] =
	{
		GL_UNSIGNED_BYTE,
		GL_UNSIGNED_SHORT
	};

	glDrawElements(GL_TRIANGLES, indexCount, TYPE_TABLE[indexType], (GLvoid*)indexOffset);

	LCT_TRACE_GL_ERROR();
}

/*
* Protected Instance
*/


//namespace grap
}
//namespace lct
}
