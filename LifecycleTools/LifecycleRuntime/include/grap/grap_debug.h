#ifndef GRAP_DEBUG_H
#define GRAP_DEBUG_H

#include <foun/foun_debug.h>

#if defined(LCT_WINDOWS) || defined(LCT_OSX)
#include <GL/glew.h>
#elif defined(LCT_IOS)
#include <OpenGLES/ES2/gl.h>
#elif defined(LCT_ANDROID)
#include <GLES2/gl2.h>
#endif

#if defined(LCT_DEBUG)

#if defined(LCT_WINDOWS) || defined(LCT_ANDROID) || defined(LCT_OSX) || defined(LCT_IOS)
#define LCT_TRACE_GL_ERROR() { GLenum glError = glGetError();  if (glError != GL_NO_ERROR) { LCT_TRACE("GL error: %d, file: %s, line: %d\n", glError, __FILE__, __LINE__); } }
#endif

#else

#define LCT_TRACE_GL_ERROR()

#endif

#endif//GRAP_DEBUG_H
