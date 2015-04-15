#ifndef FILL_RESOURCES_H
#define FILL_RESOURCES_H

#include <foun/foun_primitives.h>

#if defined(WIN32) || defined(__APPLE__)
#include <GL/glew.h>
#elif defined(__ANDROID__)
#include <GLES2/gl2.h>
#endif

namespace lct
{
namespace fill
{

#if defined(WIN32) || defined(__ANDROID__) || defined(__APPLE__)
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
