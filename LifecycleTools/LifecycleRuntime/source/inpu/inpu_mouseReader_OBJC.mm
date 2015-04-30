//
//  mouseReader_OBJC.m
//  LifecycleRuntime
//
//  Created by Michael Kron on 3/18/15.
//  Copyright (c) 2015 Michael Kron. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#import <inpu/inpu_mouseReader_OBJC.h>

NSPoint GetEventLocTopLeftOrigin(NSEvent* event)
{
	NSWindow* window = [[NSApplication sharedApplication] mainWindow];
	NSView* view = window.contentView;

	NSPoint eventLocation = [event locationInWindow];
	eventLocation = [view convertPoint:eventLocation fromView:nil];
	eventLocation.y = view.frame.size.height - eventLocation.y;
	
	return eventLocation;
}

namespace lct
{
namespace inpu
{

bool IsMouseDownEvent(void* nsEvent)
{
	NSEvent* event = (NSEvent*)nsEvent;
	
	return (event.type == NSLeftMouseDown);
}

bool IsMouseUpEvent(void* nsEvent)
{
	NSEvent* event = (NSEvent*)nsEvent;
	
	return (event.type == NSLeftMouseUp);
}

bool IsMouseMoveEvent(void* nsEvent)
{
	NSEvent* event = (NSEvent*)nsEvent;
	
	return (event.type == NSLeftMouseDragged);
}

int GetXParam(void* nsEvent)
{
	NSEvent* event = (NSEvent*)nsEvent;
	
	//NSPoint eventLocation = [event locationInWindow];
	NSPoint eventLocation = GetEventLocTopLeftOrigin(event);
	return eventLocation.x;
	//return event.mouseLocation.x;
}

int GetYParam(void* nsEvent)
{
	NSEvent* event = (NSEvent*)nsEvent;
	
	//NSPoint eventLocation = [event locationInWindow];
	NSPoint eventLocation = GetEventLocTopLeftOrigin(event);
	return eventLocation.y;
	//return event.mouseLocation.y;
}

//namespace inpu
}
//namespace lct
}