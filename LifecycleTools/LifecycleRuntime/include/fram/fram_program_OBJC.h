#ifndef FRAM_PROGRAM_OBJC_H
#define FRAM_PROGRAM_OBJC_H

namespace lct
{
namespace fram
{

struct NSAppInfo
{
	void* pWindow;
	void* pView;
	void* pGLContext;
};

struct NSScreenInfo
{
	int x;
	int y;
	int width;
	int height;
};

#if defined(LCT_OSX)
void LaunchApplication(int argc, char* argv[], void (*func)());
void RegisterUIEventCallback(void (*callback)(void*, void*), void* context);

void CreateWindow(NSAppInfo* pAppInfo, int width, int height, const char16_t* pTitle);
void CreateGLContext(NSAppInfo* pAppInfo);
void GetScreenInfo(NSScreenInfo* pScreenInfo);
void ProcessRunLoop();
void* GetNextEvent();
void SendEvent(void* nsEvent);
void SwapBuffers(NSAppInfo* pAppInfo);
bool IsWindowClosing(NSAppInfo* pAppInfo);

#elif defined(LCT_IOS)

void LaunchApplication(int argc, char* argv[], void (*callback)());
void RegisterUIEventCallback(void (*callback)(void*, void*), void* context);

void CreateWindow(NSAppInfo* pAppInfo, int width, int height, const char16_t* pTitle);
void CreateGLContext(NSAppInfo* pAppInfo);
void GetScreenInfo(NSScreenInfo* pScreenInfo);
void ProcessRunLoop();
void SwapBuffers(NSAppInfo* pAppInfo);
bool IsWindowClosing(NSAppInfo* pAppInfo);
#endif
    
//namespace fram
}
//namespace lct
}

#endif//FRAM_PROGRAM_OBJC_H
