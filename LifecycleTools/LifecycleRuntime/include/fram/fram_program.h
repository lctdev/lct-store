#ifndef FRAM_PROGRAM_H
#define FRAM_PROGRAM_H

#include <foun/foun_primitives.h>
#include <foun/foun_linearAllocator.h>
#include <foun/foun_list.h>
#include <foun/foun_keys.h>
#include <foun/foun_platform.h>

#include <fram/fram_screen.h>
#include <fram/fram_modeFactory.h>
#include <fram/fram_messageQueue.h>

#if defined(WIN32)
#include <windows.h>
#include <GL/glew.h>
#elif defined(__ANDROID__)
#include <android_native_app_glue.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#elif defined(__APPLE__)
#include <fram/fram_program_OBJC.h>
#include <GL/glew.h>
#endif

namespace lct
{
namespace fram
{

class Mode;
class Overlay;

class Program
{
private:
#if defined(WIN32)
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#elif defined(__ANDROID__)
	static void OnAppCmd(struct android_app* app, int32_t cmd);
	static int32_t OnInputEvent(struct android_app* app, AInputEvent* event);
#endif

public:
	Program();
	virtual ~Program();

	void Init();
	void Run();

#if defined(WIN32)
	void SetHInstance(HINSTANCE hInstance);
	void SetNCmdShow(int nCmdShow);
#elif defined (__ANDROID__)
	void SetAndroidApp(android_app* pAndroidApp);
#endif

protected:	
	virtual void InitMemory();
	virtual void InitConsole();
	virtual void InitFiles();
	virtual void InitGraphics();
	virtual void InitAssets();
	virtual void InitInput();
	virtual void InitWindow();
	virtual void InitModes();
	virtual void InitOverlays();
	virtual void InitMessages();

	void RegisterMode(ModeFactoryItem::CreateModeFunc createModeFunc, const char* pName);

	virtual void AcquireGraphics();
	virtual void ReleaseGraphics();

	void BeginMode();
	void EndMode();
	virtual void ReadSystemMessages();
	virtual void ReadMessages();
	virtual void ReadInput();
	virtual void Update();
	virtual void Draw();

	virtual void ConfigureMode();
	virtual bool HandlePlatformMessage(const foun::PlatformMessage& platformMessage);
	virtual bool HandleMessage(const Message& message);

	bool m_initialized;
	bool m_graphicsAcquired;

	u32 m_memorySize;
	lct::foun::LinearAllocator m_allocator;
	u32 m_modeMemorySize;
	lct::foun::LinearAllocator m_modeAllocator;

	u32 m_windowWidth;
	u32 m_windowHeight;
	const c16* m_pWindowLabel;
	lct::fram::Screen m_screen;

	bool m_running;
	u32 m_frameCount;

	ModeFactory m_modeFactory;
	Mode* m_pCurrMode;
	const char* m_pNextModeName;

	Overlay* m_pCurrOverlay;

	MessageQueue m_messageQueue;

#if defined(WIN32)
	HINSTANCE m_hInstance;
	int m_nCmdShow;
	HWND m_hWnd;
	HDC m_hDC;
#elif defined(__ANDROID__)
	android_app* m_pAndroidApp;
	EGLDisplay m_display;
	EGLSurface m_surface;
	EGLContext m_context;
#elif defined(__APPLE__)
    NSAppInfo m_nsAppInfo;
#endif
};

//namespace lct
}
//namespace fram
}

#endif//FRAM_PROGRAM_H
