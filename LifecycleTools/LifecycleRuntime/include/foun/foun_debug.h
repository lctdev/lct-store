#ifndef FOUN_DEBUG_H
#define FOUN_DEBUG_H

#if defined(LCT_WINDOWS)
#include <Windows.h>
#include <assert.h>
#include <stdio.h>
#elif defined(LCT_OSX) || defined(LCT_IOS)
#include <stdio.h>
#elif defined(LCT_ANDROID)
#include <android/log.h>
#endif

#if defined(LCT_DEBUG)

#if defined(LCT_WINDOWS)
#define LCT_TRACE(...) { char buffer[256]; sprintf_s(buffer, 256, __VA_ARGS__); wchar_t wideBuffer[256]; mbstowcs_s(NULL, wideBuffer, 256, buffer, 256); OutputDebugString(wideBuffer); }
#define LCT_ASSERT(exp) assert(exp);
#elif defined(LCT_OSX) || defined(LCT_IOS)
#define LCT_TRACE(...) printf(__VA_ARGS__); fflush(stdout)
#define LCT_ASSERT(exp) if (!(exp)) { printf("assertion failed: %s\n", #exp); fflush(stdout); __builtin_trap(); }
#elif defined(LCT_ANDROID)
#define LCT_TRACE(...) ((void)__android_log_print(ANDROID_LOG_INFO, "LCT", __VA_ARGS__));
#define LCT_ASSERT(exp) if (!(exp)) { __android_log_assert(#exp, "LCT", "assertion failed: %s\n", #exp); }
#endif

#else

#define LCT_TRACE(...)
#define LCT_ASSERT(exp)

#endif

#endif//FOUN_DEBUG_H
