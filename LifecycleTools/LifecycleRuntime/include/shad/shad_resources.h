#ifndef SHAD_RESOURCES_H
#define SHAD_RESOURCES_H

#include <foun/foun_primitives.h>

#if defined(WIN32) || defined(__APPLE__)
#include <GL/glew.h>
#elif defined(__ANDROID__)
#include <GLES2/gl2.h>
#endif

namespace lct
{
namespace shad
{

#if defined(WIN32) || defined(__ANDROID__) || defined(__APPLE__)
struct UniformResource
{
	GLint uniformLocation;
};

struct AttributeResource
{
	GLuint offset;
};

struct ShaderResource
{
	GLuint hVertexShader;
	GLuint hFragmentShader;
	GLuint hShaderProgram;
};
#endif

//namespace shad
}
//namespace lct
}

#endif//SHAD_RESOURCES_H
