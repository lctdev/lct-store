#include <fram/fram_program.h>
#include <fram/fram_mode.h>
#include <fram/fram_overlay.h>
#include <fram/fram_message.h>

#include <audi/audi_debug.h>

#include <foun/foun_debug.h>
#include <foun/foun_string.h>

#include <stdlib.h>
#if !defined(LCT_WINDOWS)
#include <unistd.h>
#endif

namespace lct
{
namespace fram
{

/*
 * Internal Constants
 */
static const u32 DEFAULT_MEMORY_SIZE = 4 * 1024 * 1024;
static const u32 DEFAULT_MODE_MEMORY_SIZE = 32 * 1024 * 1024;

static const u32 DEFAULT_WINDOW_WIDTH = 960;
static const u32 DEFAULT_WINDOW_HEIGHT = 540;
static const c16* DEFAULT_WINDOW_LABEL = LCT_UNICODE("Program");

static const u32 MESSAGE_BUFFER_SIZE = 32;

/*
 * Private Static
 */
#if defined(LCT_WINDOWS)
LRESULT CALLBACK Program::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Program* pProgram = NULL;
	if (message == WM_CREATE)
	{
		LPCREATESTRUCT cs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		pProgram = reinterpret_cast<Program*>(cs->lpCreateParams);
	}
	else
	{
		pProgram = reinterpret_cast<Program*>(GetWindowLong(hWnd, GWL_USERDATA));
	}

	// TEMP? Are there messages we are missing before the program pointer is set...?
	if (pProgram != NULL)
	{
		foun::PlatformMessage platformMessage;
		platformMessage.hWnd = hWnd;
		platformMessage.message = message;
		platformMessage.wParam = wParam;
		platformMessage.lParam = lParam;
		if (pProgram->HandlePlatformMessage(platformMessage))
		{
			return 0;
		}
		else
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	else
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}
#elif defined(LCT_ANDROID)
void Program::OnAppCmd(struct android_app* app, int32_t cmd)
{
    Program* pProgram = static_cast<Program*>(app->userData);

    foun::PlatformMessage platformMessage;
    platformMessage.cmd = cmd;
    platformMessage.inputEvent = NULL;
    pProgram->HandlePlatformMessage(platformMessage);
}

int32_t Program::OnInputEvent(struct android_app* app, AInputEvent* event)
{
	Program* pProgram = static_cast<Program*>(app->userData);

	foun::PlatformMessage platformMessage;
	platformMessage.cmd = -1;
	platformMessage.inputEvent = event;
	if (pProgram->HandlePlatformMessage(platformMessage))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
#elif defined(LCT_IOS)
void Program::OnUIEvent(void* program, void* uiEvent)
{
	Program* pProgram = static_cast<Program*>(program);

	foun::PlatformMessage platformMessage;
	platformMessage.uiEvent = uiEvent;
	pProgram->HandlePlatformMessage(platformMessage);
}
#endif

/*
 * Public Instance
 */
Program::Program()
: m_initialized(false)
, m_graphicsAcquired(false)
, m_memorySize(DEFAULT_MEMORY_SIZE)
, m_allocator()
, m_modeMemorySize(DEFAULT_MODE_MEMORY_SIZE)
, m_modeAllocator()
, m_graphicsDevice()
, m_audioDevice()
, m_windowWidth(DEFAULT_WINDOW_WIDTH)
, m_windowHeight(DEFAULT_WINDOW_HEIGHT)
, m_pWindowLabel(DEFAULT_WINDOW_LABEL)
, m_screen()
, m_running(false)
, m_frameCount(0)
, m_modeFactory()
, m_pCurrMode(NULL)
, m_pNextModeName(NULL)
, m_pOverlayArray(NULL)
, m_overlayCount(0)
, m_messageQueue()
#if defined(LCT_WINDOWS)
, m_hInstance(NULL)
, m_nCmdShow(0)
, m_hWnd(NULL)
, m_hDC(NULL)
#elif defined(LCT_ANDROID)
, m_pAndroidApp(NULL)
, m_display(NULL)
, m_surface(NULL)
, m_context(NULL)
#elif defined(LCT_OSX) || defined(LCT_IOS)
, m_nsAppInfo()
#endif
{
}

Program::~Program()
{
}

#if defined(LCT_WINDOWS)
void Program::SetHInstance(HINSTANCE hInstance)
{
	m_hInstance = hInstance;
}

void Program::SetNCmdShow(int nCmdShow)
{
	m_nCmdShow = nCmdShow;
}
#elif defined(LCT_ANDROID)
void Program::SetAndroidApp(android_app* pAndroidApp)
{
	m_pAndroidApp = pAndroidApp;
}
#endif

void Program::Init()
{
	LCT_TRACE("Program::Init\n");

	InitMemory();
	InitConsole();
	InitFiles();
	InitGraphics();
	InitAudio();
	InitAssets();
	InitInput();
	InitWindow();
	InitMiscellaneous();

	m_initialized = true;
}

void Program::Run()
{
	LCT_TRACE("Program::Run\n");

	m_running = true;
	
	ReadSystemMessages();

	while (m_running)
	{
		if (m_initialized)
		{
			ReadMessages();

			CheckModeChange();

			ReadInput();

			Update();

			if (m_graphicsAcquired)
			{
				Draw();
			}

			++m_frameCount;
		}
		else
		{
#if defined(LCT_WINDOWS)
			Sleep(100);
#else
			usleep(100);
#endif
		}
		
		ReadSystemMessages();
	}
}

/*
 * Protected Instance
 */
void Program::InitMemory()
{
	{
		void* pMemory = malloc(m_memorySize);
		m_allocator.SetMemory(pMemory, m_memorySize);
	}

	{
		void* pMemory = malloc(m_modeMemorySize);
		m_modeAllocator.SetMemory(pMemory, m_modeMemorySize);
	}
}

void Program::InitConsole()
{
}

void Program::InitFiles()
{
}

void Program::InitGraphics()
{
}

void Program::InitAudio()
{
	AcquireAudio();
}

void Program::InitAssets()
{
}

void Program::InitInput()
{
}

void Program::InitWindow()
{
#if defined(LCT_WINDOWS)
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0; 
	wcex.hInstance = m_hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1 );
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = m_pWindowLabel;
	wcex.hIconSm = NULL;
	if(!RegisterClassEx(&wcex))
	{
		//DWORD error = GetLastError();
	}

	// Create and show window
	RECT rc = { 0, 0, (LONG)m_windowWidth, (LONG)m_windowHeight };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE );
	m_hWnd = CreateWindow(m_pWindowLabel, m_pWindowLabel, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, m_hInstance, this);
	ShowWindow(m_hWnd, m_nCmdShow);

	AcquireGraphics();
#elif defined(LCT_ANDROID)
	m_pAndroidApp->userData = this;
	m_pAndroidApp->onAppCmd = OnAppCmd;
	m_pAndroidApp->onInputEvent = OnInputEvent;
#elif defined(LCT_OSX)
    CreateWindow(&m_nsAppInfo, (int)m_windowWidth, (int)m_windowHeight, m_pWindowLabel);
    
    AcquireGraphics();
#elif defined(LCT_IOS)
	CreateWindow(&m_nsAppInfo, (int)m_windowWidth, (int)m_windowHeight, m_pWindowLabel);
	RegisterUIEventCallback(OnUIEvent, this);
    
    AcquireGraphics();
#endif
}

void Program::InitMiscellaneous()
{
	void* pMemory = m_allocator.Alloc(MESSAGE_BUFFER_SIZE, 4);
	m_messageQueue.SetMemory(pMemory, MESSAGE_BUFFER_SIZE);
}

void Program::RegisterMode(ModeFactoryItem::CreateModeFunc createModeFunc, const char* pName)
{
	ModeFactoryItem* pModeFactoryItem = m_allocator.AllocType<ModeFactoryItem>();
	pModeFactoryItem->SetName(pName);
	pModeFactoryItem->SetCreateModeFunc(createModeFunc);
	m_modeFactory.AddItem(pModeFactoryItem);
}

void Program::AcquireGraphics()
{
	LCT_TRACE("Program::AcquireGraphics\n");

#if defined(LCT_WINDOWS)
	// get the device handle
	m_hDC = GetDC(m_hWnd);

	// setup display buffers
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int iFormat = ChoosePixelFormat(m_hDC, &pfd);
	SetPixelFormat(m_hDC, iFormat, &pfd);

	// create and activate opengl render context
	HGLRC hRC;
	hRC = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC, hRC);

	GLenum error = glewInit();
	if (GLEW_OK != error)
	{
		LCT_TRACE("Glew Init error");
		return;
	}
#elif defined(LCT_ANDROID)
	const EGLint configAttribs[] =
	{
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_NONE
	};
	const EGLint contextAttribs[] =
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};
	EGLint w, h, dummy, format;
	EGLint numConfigs;
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;

	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	eglInitialize(display, 0, 0);

	/* Here, the application chooses the configuration it desires. In this
	 * sample, we have a very simplified selection process, where we pick
	 * the first EGLConfig that matches our criteria */
	eglChooseConfig(display, configAttribs, &config, 1, &numConfigs);

	/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	 * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	 * As soon as we picked a EGLConfig, we can safely reconfigure the
	 * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(m_pAndroidApp->window, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, m_pAndroidApp->window, NULL);
	context = eglCreateContext(display, config, NULL, contextAttribs);

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		LCT_TRACE("Unable to eglMakeCurrent");
		return;
	}

	eglQuerySurface(display, surface, EGL_WIDTH, &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);

	m_display = display;
	m_surface = surface;
	m_context = context;

	m_windowWidth = w;
	m_windowHeight = h;
	//engine->state.angle = 0;
#elif defined(LCT_OSX)
    CreateGLContext(&m_nsAppInfo);
	
	GLenum error = glewInit();
	if (GLEW_OK != error)
	{
		LCT_TRACE("Glew Init error");
		return;
	}
#elif defined(LCT_IOS)
	CreateGLContext(&m_nsAppInfo);
	
	NSScreenInfo screenInfo;
	GetScreenInfo(&screenInfo);
	m_windowWidth = screenInfo.width;
	m_windowHeight = screenInfo.height;
#endif

	m_screen.SetWidthHeight((f32)m_windowWidth, (f32)m_windowHeight);

	m_graphicsAcquired = true;

	if (m_pCurrMode != NULL)
	{
		m_pCurrMode->AcquireGraphics();
	}

	for (u32 overlayIndex = 0; overlayIndex < m_overlayCount; ++overlayIndex)
	{
		Overlay* pOverlay = m_pOverlayArray[overlayIndex];
		pOverlay->AcquireGraphics();
	}
}

void Program::AcquireAudio()
{
	LCT_TRACE("Program::AcquireAudio\n");

#if defined(LCT_WINDOWS)
	// get the device
	ALCdevice* pDev = alcOpenDevice(NULL);

	HMODULE handle = LoadLibrary(L"OpenAL32.dll");

	// create and activate context
	ALCcontext* pCtx = alcCreateContext(pDev, NULL);
	alcMakeContextCurrent(pCtx);
	if (!pCtx)
	{
		LCT_TRACE("OpenAL create context error\n");
		LCT_TRACE_AL_ERROR();
		return;
	}
#elif defined(LCT_OSX)
	// get the device
	ALCdevice* pDev = alcOpenDevice(NULL);

	// create and activate context
	ALCcontext* pCtx = alcCreateContext(pDev, NULL);
	alcMakeContextCurrent(pCtx);
	if (!pCtx)
	{
		LCT_TRACE("OpenAL create context error");
		return;
	}
#elif defined(LCT_IOS)
	// get the device
	ALCdevice* pDev = alcOpenDevice(NULL);

	// create and activate context
	ALCcontext* pCtx = alcCreateContext(pDev, NULL);
	alcMakeContextCurrent(pCtx);
	if (!pCtx)
	{
		LCT_TRACE("OpenAL create context error");
		return;
	}
#endif

	m_audioDevice.SetAllocator(&m_allocator);
	m_audioDevice.CreateVoiceResources();
	m_audioDevice.AcquireVoiceResources();
}

void Program::ReleaseGraphics()
{
	LCT_TRACE("Program::ReleaseGraphics\n");

	if (m_pCurrMode != NULL)
	{
		m_pCurrMode->ReleaseGraphics();
	}

	for (u32 overlayIndex = 0; overlayIndex < m_overlayCount; ++overlayIndex)
	{
		Overlay* pOverlay = m_pOverlayArray[overlayIndex];
		pOverlay->AcquireGraphics();
	}

#if defined(LCT_WINDOWS)
	// TODO
#elif defined(LCT_ANDROID)
	if (m_display != EGL_NO_DISPLAY) {
		eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (m_context != EGL_NO_CONTEXT) {
			eglDestroyContext(m_display, m_context);
		}
		if (m_surface != EGL_NO_SURFACE) {
			eglDestroySurface(m_display, m_surface);
		}
		eglTerminate(m_display);
	}
	m_display = EGL_NO_DISPLAY;
	m_surface = EGL_NO_SURFACE;
	m_context = EGL_NO_CONTEXT;
#endif

	m_graphicsAcquired = false;
}

void Program::ReadSystemMessages()
{
#if defined(LCT_WINDOWS)
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			m_running = false;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
#elif defined(LCT_ANDROID)
	int ident;
	int events;
	struct android_poll_source* source;

	while ((ident = ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0)
	{
		// Process this event ( will invoke our command handler )
		if (source != NULL) {
			source->process(m_pAndroidApp, source);
		}
	}
#elif defined(LCT_OSX)
	void* nsEvent;
	while ((nsEvent = GetNextEvent()) != NULL)
	{
		foun::PlatformMessage platformMessage;
		platformMessage.nsEvent = nsEvent;
		
		HandlePlatformMessage(platformMessage);
	
		SendEvent(nsEvent);
	}
	
	if (IsWindowClosing(&m_nsAppInfo))
	{
		m_running = false;
	}
#elif defined(LCT_IOS)
	ProcessRunLoop();
#endif
}

void Program::ReadMessages()
{
	Message message;
	while (m_messageQueue.GetFrontMessage(message))
	{
		HandleMessage(message);

		m_messageQueue.DequeueMessage();
	}
}

void Program::CheckModeChange()
{
	if (m_pCurrMode == NULL)
	{
		BeginMode();
	}
}

void Program::ReadInput()
{
	bool inputBlocked = false;
	for (u32 overlayIndex = 0; overlayIndex < m_overlayCount; ++overlayIndex)
	{
		Overlay* pOverlay = m_pOverlayArray[overlayIndex];
		if (pOverlay->IsActive() && !inputBlocked)
		{
			pOverlay->ReadInput();
			if (pOverlay->DoesBlockInput())
			{
				inputBlocked = true;
			}
		}
	}

	if (!inputBlocked)
	{
		m_pCurrMode->ReadInput();
	}
}

void Program::Update()
{
	m_pCurrMode->Update();

	for (u32 overlayIndex = 0; overlayIndex < m_overlayCount; ++overlayIndex)
	{
		Overlay* pOverlay = m_pOverlayArray[overlayIndex];
		if (pOverlay->IsActive())
		{
			pOverlay->Update();
		}
	}
}

void Program::Draw()
{
	// clear the back buffer
	const lct::foun::FloatColor4& clearColor = m_pCurrMode->GetClearColor();
	m_graphicsDevice.ClearFrameBuffer(clearColor);

	m_pCurrMode->Draw();

	for (u32 overlayIndex = 0; overlayIndex < m_overlayCount; ++overlayIndex)
	{
		Overlay* pOverlay = m_pOverlayArray[overlayIndex];
		if (pOverlay->IsActive())
		{
			pOverlay->Draw();
		}
	}

#if defined(LCT_WINDOWS)
	// swap the back buffer
	SwapBuffers(m_hDC);
#elif defined(LCT_ANDROID)
	eglSwapBuffers(m_display, m_surface);
#elif defined(LCT_OSX) || defined(LCT_IOS)
	SwapBuffers(&m_nsAppInfo);
	//WaitVsync();
#endif
}

void Program::BeginMode()
{
	LCT_TRACE("Program::BeginMode\n");

	m_pCurrMode = m_modeFactory.CreateMode(m_pNextModeName, &m_modeAllocator);
	ConfigureMode();
	m_pCurrMode->Init();
	if (m_graphicsAcquired)
	{
		m_pCurrMode->AcquireGraphics();
	}
	m_pCurrMode->AcquireAudio();
	m_pNextModeName = NULL;
}

void Program::EndMode()
{
	LCT_TRACE("Program::EndMode\n");

	m_pCurrMode->ReleaseAudio();
	if (m_graphicsAcquired)
	{
		m_pCurrMode->ReleaseGraphics();
	}
	m_pCurrMode = NULL;
	m_modeAllocator.Clear();
}

void Program::ConfigureMode()
{
	Mode::Shared modeShared;
	modeShared.pAllocator = &m_modeAllocator;
	modeShared.pProgramMessageQueue = &m_messageQueue;
	modeShared.pScreen = &m_screen;
	modeShared.pGraphicsDevice = &m_graphicsDevice;
	modeShared.pAudioDevice = &m_audioDevice;
	m_pCurrMode->SetShared(modeShared);
}

void Program::ActivateOverlay(u32 index)
{
	if (index >= m_overlayCount)
	{
		return;
	}

	m_pOverlayArray[index]->Activate();
}

void Program::DeactivateOverlay(u32 index)
{
	if (index >= m_overlayCount)
	{
		return;
	}

	m_pOverlayArray[index]->Deactivate();
}

bool Program::IsOverlayActive(u32 index)
{
	if (index >= m_overlayCount)
	{
		return false;
	}

	return m_pOverlayArray[index]->IsActive();
}

bool Program::HandlePlatformMessage(const foun::PlatformMessage& platformMessage)
{
#if defined(LCT_WINDOWS)
	switch(platformMessage.message)
	{
	case WM_CREATE:
	{
		// store pointer to ourself
		SetWindowLong(platformMessage.hWnd, GWL_USERDATA, (LONG)this);

		//pProgram->AcquireGraphics();

		return true;
	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(platformMessage.hWnd, &ps);
		EndPaint(platformMessage.hWnd, &ps);

		return true;
	}
	break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);

		//pProgram->ReleaseGraphics();

		return true;
	}
	break;

	default:
		return false;
	}
#elif defined(LCT_ANDROID)
	if (platformMessage.cmd != -1)
	{
		switch (platformMessage.cmd)
	    {
	        case APP_CMD_INIT_WINDOW:
	        	LCT_TRACE("APP_CMD_INIT_WINDOW\n");
	            // The window is being shown, get it ready.
	            if (m_pAndroidApp->window != NULL)
	            {
	                AcquireGraphics();
	            }
	            break;
	        case APP_CMD_TERM_WINDOW:
	        	LCT_TRACE("APP_CMD_TERM_WINDOW\n");
	            // The window is being hidden or closed, clean it up.
	            ReleaseGraphics();
	            break;
	        case APP_CMD_START:
	        	LCT_TRACE("APP_CMD_START\n");
	        	break;
	        case APP_CMD_RESUME:
	        	LCT_TRACE("APP_CMD_RESUME\n");
	        	break;
	        case APP_CMD_SAVE_STATE:
	        	LCT_TRACE("APP_CMD_SAVE_STATE\n");
	        	break;
	        case APP_CMD_PAUSE:
	        	LCT_TRACE("APP_CMD_PAUSE\n");
	        	break;
	        case APP_CMD_STOP:
	        	LCT_TRACE("APP_CMD_STOP\n");
	        	break;
	        case APP_CMD_DESTROY:
	        	LCT_TRACE("APP_CMD_DESTROY\n");
	        	m_running = false;
	        	break;
	    }
	}
#elif defined (__APPPLE__)
	// TODO: check for quit
	
	return false;
#else
    return false;
#endif
}

bool Program::HandleMessage(const Message& message)
{
	return false;
}

//namespace lct
}
//namespace fram
}
