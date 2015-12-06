//
//  mouseReader_OBJC.m
//  LifecycleRuntime
//
//  Created by Michael Kron on 3/18/15.
//  Copyright (c) 2015 Michael Kron. All rights reserved.
//

#import <Foundation/Foundation.h>
#if defined(LCT_IOS)
#import <UIKit/UIKit.h>
#endif

#import <inpu/inpu_touchReader_OBJC.h>

#if defined(LCT_IOS)
CGPoint GetEventLocTopLeftOrigin(UIEvent* event)
{
	NSSet* allTouches = [event allTouches];
	UITouch* touch = [allTouches anyObject];
	UIView* view = [touch view];
	
	CGPoint location = [touch locationInView:view];
	CGFloat screenScale = [[UIScreen mainScreen] scale];

	location.x = location.x * screenScale;
	location.y = location.y * screenScale;
	return location;
}

int GetTouchPhase(UIEvent* event)
{
	if (event.type == UIEventTypeTouches)
	{
		NSSet* allTouches = [event allTouches];
		UITouch* touch = [allTouches anyObject];
		if (touch != nil)
		{
			return touch.phase;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

namespace lct
{
namespace inpu
{

bool IsTouchDownEvent(void* uiEvent)
{
	UIEvent* event = (__bridge UIEvent*)uiEvent;
	
	int phase = GetTouchPhase(event);
	if (phase == UITouchPhaseBegan)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool IsTouchUpEvent(void* uiEvent)
{
	UIEvent* event = (__bridge UIEvent*)uiEvent;
	
	int phase = GetTouchPhase(event);
	if ((phase == UITouchPhaseEnded) || (phase == UITouchPhaseCancelled))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool IsTouchMoveEvent(void* uiEvent)
{
	UIEvent* event = (__bridge UIEvent*)uiEvent;
	
	int phase = GetTouchPhase(event);
	if (phase == UITouchPhaseMoved)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int GetXParam(void* uiEvent)
{
	UIEvent* event = (__bridge UIEvent*)uiEvent;
	
	CGPoint location = GetEventLocTopLeftOrigin(event);
	return location.x;
}

int GetYParam(void* uiEvent)
{
	UIEvent* event = (__bridge UIEvent*)uiEvent;
	
	CGPoint location = GetEventLocTopLeftOrigin(event);
	return location.y;
}

void LogEvent(void* uiEvent)
{
	UIEvent* event = (__bridge UIEvent*)uiEvent;
	
	if (event.type == UIEventTypeTouches)
	{
		NSLog(@"Touch Event");
		NSSet* allTouches = [event allTouches];
		UITouch* touch = [allTouches anyObject];
		if (touch != nil)
		{
			NSLog(@"Phase: %i", (int)touch.phase);
			//return touch.phase;
		}
		else
		{
			//return -1;
		}
	}
	else
	{
		NSLog(@"Other Event");
	}
}

//namespace inpu
}
//namespace lct
}
#endif