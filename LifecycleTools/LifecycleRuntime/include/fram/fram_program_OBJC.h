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

void CreateWindow(NSAppInfo* pAppInfo, int width, int height, const char16_t* pTitle);
void CreateGLContext(NSAppInfo* pAppInfo);
void* GetNextEvent();
void SendEvent(void* nsEvent);
void SwapBuffers(NSAppInfo* pAppInfo);
bool IsWindowClosing(NSAppInfo* pAppInfo);
    
//namespace fram
}
//namespace lct
}

#endif//FRAM_PROGRAM_OBJC_H
