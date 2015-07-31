#ifndef AUDI_DEBUG_H
#define AUDI_DEBUG_H

#include <foun/foun_debug.h>

#if defined(LCT_WINDOWS)
#include <AL/al.h>
#include <AL/alc.h>
#endif

#if defined(LCT_DEBUG)

#if defined(LCT_WINDOWS)
#define LCT_TRACE_AL_ERROR() { ALenum alError = alGetError();  if (alError != AL_NO_ERROR) { LCT_TRACE("AL error: %d, file: %s, line: %d\n", alError, __FILE__, __LINE__); } }
#endif

#else

#define LCT_TRACE_AL_ERROR()

#endif

#endif//AUDI_DEBUG_H
