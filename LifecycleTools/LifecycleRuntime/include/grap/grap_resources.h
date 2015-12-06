#ifndef GRAP_RESOURCES_H
#define GRAP_RESOURCES_H

#include <foun/foun_primitives.h>

#if defined(LCT_WINDOWS) || defined(LCT_OSX)
#include <GL/glew.h>
#elif defined(LCT_IOS)
#include <OpenGLES/ES2/gl.h>
#elif defined(LCT_ANDROID)
#include <GLES2/gl2.h>
#endif

namespace lct
{
namespace grap
{

struct AttributeData;

struct UniformResource
{
	GLint uniformLocation;
};

struct ShaderResource
{
	GLuint hVertexShader;
	GLuint hFragmentShader;
	GLuint hShaderProgram;
};

struct VertexResource
{
	GLuint hVertexBuffer;
	u32 vertexStride;
	AttributeData* pAttributeDataArray;
	u32 attributeCount;
};

struct IndexResource
{
	GLuint hIndexBuffer;
};

struct TextureResource
{
	GLuint hTexture;
};

//namespace grap
}
//namespace lct
}

#endif//GRAP_RESOURCES_H
