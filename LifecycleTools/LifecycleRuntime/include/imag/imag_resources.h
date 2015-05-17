#ifndef IMAG_RESOURCES_H
#define IMAG_RESOURCES_H

#include <foun/foun_primitives.h>

#if defined(LCT_WINDOWS) || defined(LCT_OSX)
#include <GL/glew.h>
#elif defined(LCT_ANDROID)
#include <GLES2/gl2.h>
#endif

namespace lct
{
namespace imag
{

#if defined(LCT_WINDOWS) || defined(LCT_ANDROID) || defined(LCT_OSX)
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
