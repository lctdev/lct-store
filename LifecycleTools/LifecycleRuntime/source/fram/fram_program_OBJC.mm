//
//  programOBJC.m
//  LifecycleRuntime
//
//  Created by Michael Kron on 3/9/15.
//  Copyright (c) 2015 Michael Kron. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <iostream>

#import <fram/fram_program_OBJC.h>

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

namespace lct
{
namespace fram
{

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
	//const char* cString = (const char*)pTitle;
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

//namespace fram
}
//namespace lct
}
