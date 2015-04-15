#include <shad/shad_resourceHandler.h>
#include <shad/shad_data.h>
#include <shad/shad_resources.h>

#include <foun/foun_debug.h>
#include <foun/foun_graphicsDebug.h>

#include <string.h>

namespace lct
{
namespace shad
{

/*
 * Internal Constants
 */

/*
 * Public Instance
 */
ResourceHandler::ResourceHandler()
: m_pAllocator(NULL)
{
}

void ResourceHandler::SetAllocator(lct::foun::Allocator* pAllocator)
{
	m_pAllocator = pAllocator;
}

//void ResourceHandler::CreateShaderResources(ShaderAsset& shaderAsset)
//{
//	const ShaderData* pShaderData = shaderAsset.pShaderData;
//
//	ShaderResource* pShaderResource = m_pAllocator->AllocType<ShaderResource>();
//	memset(pShaderResource, 0, sizeof(ShaderResource));
//
//	UniformResource* pUniformResourceArray = m_pAllocator->AllocTypeArray<UniformResource>(pShaderData->uniformCount);
//	memset(pUniformResourceArray, -1, sizeof(UniformResource));
//
//	shaderAsset.pShaderResource = pShaderResource;
//	shaderAsset.pUniformResourceArray = pUniformResourceArray;
//}

void ResourceHandler::AcquireShaderResources(ShaderResource* pShaderResource, ShaderData* pShaderData, void* pVertexShaderBinary, void* pFragmentShaderBinary,
		AttributeResource* pAttributeResourceArray, AttributeData* pAttributeDataArray)
{
	char log[1024];
	s32 logLength = 0;

	pShaderResource->hVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(pShaderResource->hVertexShader, 1, (const GLchar**)&pVertexShaderBinary, 0);
	glCompileShader(pShaderResource->hVertexShader);
	FOUN_TRACE_GL_ERROR();
	glGetShaderInfoLog(pShaderResource->hVertexShader, sizeof(log), &logLength, log);
	if (logLength > 0)
	{
		LCT_TRACE("Vertex Shader log:\n");
		LCT_TRACE("%s\n", log);
	}

	pShaderResource->hFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(pShaderResource->hFragmentShader, 1, (const GLchar**)&pFragmentShaderBinary, 0);
	glCompileShader(pShaderResource->hFragmentShader);
	FOUN_TRACE_GL_ERROR();
	glGetShaderInfoLog(pShaderResource->hFragmentShader, sizeof(log), &logLength, log);
	if (logLength > 0)
	{
		LCT_TRACE("Fragment Shader log:\n");
		LCT_TRACE("%s\n", log);
	}

	pShaderResource->hShaderProgram = glCreateProgram();
	glAttachShader(pShaderResource->hShaderProgram, pShaderResource->hVertexShader);
	glAttachShader(pShaderResource->hShaderProgram, pShaderResource->hFragmentShader);

	GLuint attributeOffset = 0;
	for (u32 attributeIndex = 0; attributeIndex < pShaderData->attributeCount; ++attributeIndex)
	{
		const AttributeData* pAttributeData = pAttributeDataArray + attributeIndex;

		glBindAttribLocation(pShaderResource->hShaderProgram, attributeIndex, pAttributeData->name);

		AttributeResource* pAttributeResource = pAttributeResourceArray + attributeIndex;
		pAttributeResource->offset = attributeOffset;
		u32 attributeSize = pAttributeData->componentCount * sizeof(f32);
		attributeOffset += attributeSize;
	}

	glLinkProgram(pShaderResource->hShaderProgram);

	FOUN_TRACE_GL_ERROR();
}

void ResourceHandler::ReleaseShaderResources(ShaderResource* pShaderResource, ShaderData* pShaderData, AttributeResource* pAttributeResourceArray)
{
	glDetachShader(pShaderResource->hShaderProgram, pShaderResource->hVertexShader);
	glDetachShader(pShaderResource->hShaderProgram, pShaderResource->hFragmentShader);
	glDeleteProgram(pShaderResource->hShaderProgram);

	glDeleteShader(pShaderResource->hVertexShader);

	glDeleteShader(pShaderResource->hFragmentShader);

	memset(pShaderResource, 0, sizeof(ShaderResource));

	memset(pAttributeResourceArray, 0, (sizeof(AttributeResource) * pShaderData->attributeCount));

	FOUN_TRACE_GL_ERROR();
}

void ResourceHandler::AcquireUniformResource(ShaderResource* pShaderResource, UniformResource* pUniformResource, UniformData* pUniformData)
{
	pUniformResource->uniformLocation = glGetUniformLocation(pShaderResource->hShaderProgram, pUniformData->name);

	FOUN_TRACE_GL_ERROR();
}

void ResourceHandler::ReleaseUniformResource(UniformResource* pUniformResource)
{
	memset(pUniformResource, -1, sizeof(UniformResource));
}

//namespace shad
}
//namespace lct
}
