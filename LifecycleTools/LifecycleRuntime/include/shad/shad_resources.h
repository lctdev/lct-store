#ifndef SHAD_RESOURCES_H
#define SHAD_RESOURCES_H

#include <foun/foun_primitives.h>

#if defined(LCT_WINDOWS) || defined(LCT_OSX)
#include <GL/glew.h>
#elif defined(LCT_ANDROID)
#include <GLES2/gl2.h>
#endif

namespace lct
{
namespace shad
{

#if defined(LCT_WINDOWS) || defined(LCT_ANDROID) || defined(LCT_OSX)
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
