#ifndef FOUN_PLATFORM_H
#define FOUN_PLATFORM_H

#include <foun/foun_primitives.h>

#if defined(LCT_WINDOWS)
#include <windows.h>
#elif defined(LCT_ANDROID)
#include <android_native_app_glue.h>
#endif

namespace lct
{
namespace foun
{

struct PlatformMessage
{
#if defined(LCT_WINDOWS)
	HWND hWnd;
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
#elif defined(LCT_ANDROID)
	int32_t cmd;
	AInputEvent* inputEvent;
#elif defined(LCT_OSX)
	void* nsEvent;
#endif
};

//namespace foun
}
//namespace lct
}

#endif//FOUN_PLATFORM_H
