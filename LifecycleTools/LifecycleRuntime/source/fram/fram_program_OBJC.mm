//
//  programOBJC.m
//  LifecycleRuntime
//
//  Created by Michael Kron on 3/9/15.
//  Copyright (c) 2015 Michael Kron. All rights reserved.
//

#import <Foundation/Foundation.h>
#if defined(LCT_OSX)
#import <Cocoa/Cocoa.h>
#elif defined(LCT_IOS)
#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#endif
#import <iostream>

#import <fram/fram_program_OBJC.h>

#if defined(LCT_OSX)
@interface MyApplicationDelegate : NSObject <NSApplicationDelegate>
@end

@implementation MyApplicationDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)notification
{
    [NSApp stop:nil];
}

@end

@interface MyWindowDelegate : NSObject <NSWindowDelegate> {
	bool _closing;
}

@property bool closing;

@end

@implementation MyWindowDelegate

@synthesize closing = _closing;

- (void)windowWillClose:(id)sender
{
    self.closing = true;
}
@end
#elif defined(LCT_IOS)
void (*gLaunchCallback)();

void (*gUIEventCallback)(void*, void*) = NULL;
void* gUIEventContext = NULL;

bool gWaitingVsync = true;



@interface MyApplication : UIApplication

@end

@implementation MyApplication

CADisplayLink *mVsyncLink;

- (void)sendEvent:(UIEvent *)event
{
	if (gUIEventCallback != NULL)
	{
		gUIEventCallback(gUIEventContext, (__bridge void*)event);
	}

	[super sendEvent:event];
}

- (void)createDisplayLink
{
	mVsyncLink = [CADisplayLink displayLinkWithTarget:self
        selector:@selector(onVsync:)];
    mVsyncLink.frameInterval = 1;
    [mVsyncLink addToRunLoop:[NSRunLoop mainRunLoop]
        forMode:NSDefaultRunLoopMode];
}

- (void) onVsync:(id)data
{
	gWaitingVsync = false;
}

@end

@interface MyApplicationDelegate : NSObject <UIApplicationDelegate>
@end

@implementation MyApplicationDelegate

- (void)applicationDidFinishLaunching:(UIApplication *)application
{
	[self performSelector:@selector(onLaunch:) withObject:nil afterDelay:0.2f];
}

- (void)onLaunch:(id)sender
{       
	gLaunchCallback();
}

@end

#endif

namespace lct
{
namespace fram
{

#if defined(LCT_OSX)
void CreateWindow(NSAppInfo* pAppInfo, int width, int height, const char16_t* pTitle)
{
    NSApplication* application = [NSApplication sharedApplication];
    
    MyApplicationDelegate* appDelegate = [[MyApplicationDelegate alloc] init];
    
    [application setDelegate:appDelegate];
    [NSApp run];
    
    
    NSRect contentRect = NSMakeRect(0, 0, width, height);
    unsigned int styleMask = NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask;
    
    NSWindow* window = [[NSWindow alloc]
                        initWithContentRect:contentRect
                        styleMask:styleMask
                        backing:NSBackingStoreBuffered
                        defer:NO];
    
    [window center];
    [window setLevel:NSFloatingWindowLevel];
    
    MyWindowDelegate* windowDelegate = [[MyWindowDelegate alloc] init];
	int titleLength = std::char_traits<char16_t>::length(pTitle);
	int titleSize = titleLength * sizeof(char16_t);
	NSString* title = [[NSString alloc]
						initWithBytes:pTitle
						length:titleSize
						encoding:NSUTF16LittleEndianStringEncoding];
    
    [window setTitle:title];
    [window setDelegate:windowDelegate];
    [window orderFront:nil];
    [window setHidesOnDeactivate:true];
	
	pAppInfo->pWindow = window;
}

void CreateGLContext(NSAppInfo* pAppInfo)
{
	NSWindow* window = (NSWindow*)pAppInfo->pWindow;

    NSOpenGLPixelFormatAttribute attributes[] =
    {
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFADepthSize, 32,
		NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersionLegacy, // 1.2
        0
    };
    
    NSOpenGLPixelFormat* format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes];
    
    NSOpenGLContext* context = [[NSOpenGLContext alloc] initWithFormat:format shareContext:NULL];
    
    NSOpenGLView* view = [[NSOpenGLView alloc] init];
    [view setOpenGLContext:context];
    [view setPixelFormat:format];
    
    [context setView:view];
    [context makeCurrentContext];
    
    [window setContentView:view];
	
	pAppInfo->pView = view;
	pAppInfo->pGLContext = context;
}

void* GetNextEvent()
{
	NSEvent* event = [NSApp nextEventMatchingMask:NSAnyEventMask
                                                untilDate:[NSDate distantPast]
                                                   inMode:NSDefaultRunLoopMode
                                                  dequeue:YES];
	
	return event;
}

void SendEvent(void* nsEvent)
{
	NSEvent* event = (NSEvent*)nsEvent;
	
	[NSApp sendEvent:event];
}

void SwapBuffers(NSAppInfo* pAppInfo)
{
	NSOpenGLContext* context = (NSOpenGLContext*)pAppInfo->pGLContext;
	
	[context flushBuffer];
}

bool IsWindowClosing(NSAppInfo* pAppInfo)
{
	NSWindow* window = (NSWindow*)pAppInfo->pWindow;
	
	MyWindowDelegate* delegate = window.delegate;
	return delegate.closing;
}

#elif defined(LCT_IOS)
void LaunchApplication(int argc, char* argv[], void (*callback)())
{
	gLaunchCallback = callback;

	UIApplicationMain(argc, argv, NSStringFromClass([MyApplication class]), NSStringFromClass([MyApplicationDelegate class]));
}

void RegisterUIEventCallback(void (*callback)(void*, void*), void* context)
{
	gUIEventCallback = callback;
	gUIEventContext = context;
}

void CreateWindow(NSAppInfo* pAppInfo, int width, int height, const char16_t* pTitle)
{
    UIWindow* window = [[UIWindow alloc]
                        initWithFrame:[[UIScreen mainScreen] bounds]];
	
	pAppInfo->pWindow = (__bridge_retained void*)window;
}

void CreateGLContext(NSAppInfo* pAppInfo)
{
	UIWindow* window = (__bridge UIWindow*)pAppInfo->pWindow;
	
	EAGLContext* context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
	
	GLKView* view = [[GLKView alloc]
						initWithFrame:[[UIScreen mainScreen] bounds]];
	view.context = context;
	view.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    view.drawableStencilFormat = GLKViewDrawableStencilFormat8;
	
	UIViewController* viewController = [[UIViewController alloc] init];
	viewController.view = view;
	window.rootViewController = viewController;
	[window makeKeyAndVisible];
	
	[EAGLContext setCurrentContext:context];
	
	pAppInfo->pView = (__bridge_retained void*)view;
	pAppInfo->pGLContext = (__bridge_retained void*)context;
	
	MyApplication* application = (MyApplication*)[UIApplication sharedApplication];
	
	[application createDisplayLink];
}

void GetScreenInfo(NSScreenInfo* pScreenInfo)
{
	CGRect screenBounds = [[UIScreen mainScreen] bounds];
	CGFloat screenScale = [[UIScreen mainScreen] scale];
	
	pScreenInfo->x = screenBounds.origin.x;
	pScreenInfo->y = screenBounds.origin.y;
	pScreenInfo->width = screenBounds.size.width * screenScale;
	pScreenInfo->height = screenBounds.size.height * screenScale;
}

void ProcessRunLoop()
{
	while (gWaitingVsync)
	{
		SInt32 result;
		do
		{
			result = CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, TRUE);
		} while(result == kCFRunLoopRunHandledSource);
	}
	gWaitingVsync = true;
}

void SwapBuffers(NSAppInfo* pAppInfo)
{
	EAGLContext* context = (__bridge EAGLContext*)pAppInfo->pGLContext;
	
	[context presentRenderbuffer:GL_RENDERBUFFER];
}

bool IsWindowClosing(NSAppInfo* pAppInfo)
{
	return false;
}

#endif

//namespace fram
}
//namespace lct
}
