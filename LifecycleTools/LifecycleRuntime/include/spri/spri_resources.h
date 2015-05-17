#ifndef SPRI_RESOURCES_H
#define SPRI_RESOURCES_H

#include <foun/foun_primitives.h>

#if defined(LCT_WINDOWS) || defined(LCT_OSX)
#include <GL/glew.h>
#elif defined(LCT_ANDROID)
#include <GLES2/gl2.h>
#endif

namespace lct
{
namespace spri
{

#if defined(LCT_WINDOWS) || defined(LCT_ANDROID) || defined(LCT_OSX)
struct QuadResource
{
	GLuint hVertexBuffer;
	GLuint hIndexBuffer;
};
#endif

//namespace spri
}
//namespace lct
}

#endif//SPRI_RESOURCES_H
