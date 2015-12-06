#ifndef FRAM_PROGRAM_H
#define FRAM_PROGRAM_H

#include <foun/foun_primitives.h>
#include <foun/foun_linearAllocator.h>
#include <foun/foun_list.h>
#include <foun/foun_keys.h>
#include <foun/foun_platform.h>

#include <grap/grap_screen.h>
#include <grap/grap_device.h>

#include <audi/audi_device.h>

#include <fram/fram_modeFactory.h>
#include <fram/fram_messageQueue.h>

#if defined(LCT_WINDOWS)
#include <windows.h>
#include <GL/glew.h>
#include <AL/al.h>
#include <AL/alc.h>
#elif defined(LCT_ANDROID)
#include <android_native_app_glue.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#elif defined(LCT_OSX)
#include <fram/fram_program_OBJC.h>
#include <GL/glew.h>
#elif defined(LCT_IOS)
#include <fram/fram_program_OBJC.h>
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
#if defined(LCT_WINDOWS)
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#elif defined(LCT_ANDROID)
	static void OnAppCmd(struct android_app* app, int32_t cmd);
	static int32_t OnInputEvent(struct android_app* app, AInputEvent* event);
#elif defined(LCT_IOS)
	static void OnUIEvent(void* program, void* uiEvent);
#endif

public:
	Program();
	virtual ~Program();

	void Init();
	void Run();

#if defined(LCT_WINDOWS)
	void SetHInstance(HINSTANCE hInstance);
	void SetNCmdShow(int nCmdShow);
#elif defined (LCT_ANDROID)
	void SetAndroidApp(android_app* pAndroidApp);
#endif

protected:	
	virtual void InitMemory();
	virtual void InitConsole();
	virtual void InitFiles();
	virtual void InitGraphics();
	virtual void InitAudio();
	virtual void InitAssets();
	virtual void InitInput();
	virtual void InitWindow();
	virtual void InitMiscellaneous();

	void RegisterMode(ModeFactoryItem::CreateModeFunc createModeFunc, const char* pName);

	virtual void AcquireGraphics();
	virtual void ReleaseGraphics();
	virtual void AcquireAudio();	

	virtual void ReadSystemMessages();	
	virtual void ReadMessages();
	virtual void CheckModeChange();
	virtual void ReadInput();
	virtual void Update();
	virtual void Draw();

	void BeginMode();
	void EndMode();

	virtual void ConfigureMode();
	virtual bool HandlePlatformMessage(const foun::PlatformMessage& platformMessage);
	virtual bool HandleMessage(const Message& message);

	bool m_initialized;
	bool m_graphicsAcquired;

	u32 m_memorySize;
	foun::LinearAllocator m_allocator;
	u32 m_modeMemorySize;
	foun::LinearAllocator m_modeAllocator;

	grap::Device m_graphicsDevice;

	audi::Device m_audioDevice;

	u32 m_windowWidth;
	u32 m_windowHeight;
	const c16* m_pWindowLabel;
	grap::Screen m_screen;

	bool m_running;
	u32 m_frameCount;

	ModeFactory m_modeFactory;
	Mode* m_pCurrMode;
	const char* m_pNextModeName;

	Overlay* m_pCurrOverlay;

	MessageQueue m_messageQueue;

#if defined(LCT_WINDOWS)
	HINSTANCE m_hInstance;
	int m_nCmdShow;
	HWND m_hWnd;
	HDC m_hDC;
#elif defined(LCT_ANDROID)
	android_app* m_pAndroidApp;
	EGLDisplay m_display;
	EGLSurface m_surface;
	EGLContext m_context;
#elif defined(LCT_OSX) || defined(LCT_IOS)
    NSAppInfo m_nsAppInfo;
#endif
};

//namespace lct
}
//namespace fram
}

#endif//FRAM_PROGRAM_H
