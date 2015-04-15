#ifndef FOUN_PLATFORM_H
#define FOUN_PLATFORM_H

#include <foun/foun_primitives.h>

#if defined(WIN32)
#include <windows.h>
#elif defined(__ANDROID__)
#include <android_native_app_glue.h>
#endif

namespace lct
{
namespace foun
{

struct PlatformMessage
{
#if defined(WIN32)
	HWND hWnd;
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
#elif defined(__ANDROID__)
	int32_t cmd;
	AInputEvent* inputEvent;
#elif defined(__APPLE__)
	void* nsEvent;
#endif
};

//namespace foun
}
//namespace lct
}

#endif//FOUN_PLATFORM_H
