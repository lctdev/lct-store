#ifndef IMAG_RESOURCES_H
#define IMAG_RESOURCES_H

#include <foun/foun_primitives.h>

#if defined(WIN32) || defined(__APPLE__)
#include <GL/glew.h>
#elif defined(__ANDROID__)
#include <GLES2/gl2.h>
#endif

namespace lct
{
namespace imag
{

#if defined(WIN32) || defined(__ANDROID__) || defined(__APPLE__)
struct TextureResource
{
	GLuint hTexture;
};
#endif

//namespace imag
}
//namespace lct
}

#endif//IMAG_RESOURCES_H
