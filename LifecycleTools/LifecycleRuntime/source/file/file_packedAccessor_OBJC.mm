//
//  file_packedAccessor_OBJC.m
//  LifecycleRuntime
//
//  Created by Michael Kron on 10/29/15.
//  Copyright © 2015 Michael Kron. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <file/file_packedAccessor_OBJC.h>

namespace lct
{
namespace file
{

bool ConvertResourcePath(const char* pFilePath, char* pResourcePath, int resourcePathSize)
{
	NSString* filePath = [NSString stringWithCString:pFilePath encoding:NSASCIIStringEncoding];
	NSString* resourcePath = [[NSBundle mainBundle] pathForResource:filePath ofType:@""];
	if (resourcePath == nil)
	{
		*pResourcePath = '\0';
		return false;
	}
	if (strlen(resourcePath.UTF8String) >= resourcePathSize)
	{
		*pResourcePath = '\0';
		return false;
	}
	strcpy(pResourcePath, resourcePath.UTF8String);
	return true;
	// TODO: free strings?
}

//namespace file
}
//namespace lct
}