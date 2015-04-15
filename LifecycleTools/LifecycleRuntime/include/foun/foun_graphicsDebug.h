#ifndef FOUN_GRAPHICS_DEBUG_H
#define FOUN_GRAPHICS_DEBUG_H

#include <foun/foun_debug.h>

#if defined(WIN32) || defined(__APPLE__)
#include <GL/glew.h>
#elif defined(__ANDROID__)
#include <GLES2/gl2.h>
#endif

#if defined(LCT_DEBUG)

#if defined(WIN32) || defined(__ANDROID__) || defined(__APPLE__)
#define FOUN_TRACE_GL_ERROR() { GLenum glError = glGetError();  if (glError != GL_NO_ERROR) { LCT_TRACE("GL error: %d, file: %s, line: %d\n", glError, __FILE__, __LINE__); } }
#endif

#else

#define FOUN_TRACE_GL_ERROR()

#endif

#endif//FOUN_GRAPHICS_DEBUG_H
