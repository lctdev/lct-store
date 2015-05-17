#ifndef FILL_RESOURCES_H
#define FILL_RESOURCES_H

#include <foun/foun_primitives.h>

#if defined(LCT_WINDOWS) || defined(LCT_OSX)
#include <GL/glew.h>
#elif defined(LCT_ANDROID)
#include <GLES2/gl2.h>
#endif

namespace lct
{
namespace fill
{

#if defined(LCT_WINDOWS) || defined(LCT_ANDROID) || defined(LCT_OSX)
struct QuadResource
{
	GLuint hVertexBuffer;
	GLuint hIndexBuffer;
};
#endif

//namespace fill
}
//namespace lct
}

#endif//FILL_RESOURCES_H
