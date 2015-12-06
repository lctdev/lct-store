//
//  main.m
//  AssetViewer_IOS
//
//  Created by Michael Kron on 10/23/15.
//  Copyright © 2015 Michael Kron. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"

#include <fram/fram_program_OBJC.h>
#include <assetViewerProgram.h>

void programMain() {
	AssetViewerProgram* pProgram = new AssetViewerProgram();
    pProgram->Init();
    pProgram->Run();
    delete pProgram;
}

int main(int argc, char * argv[]) {
	//@autoreleasepool {
	//    return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
	//}
	
	lct::fram::LaunchApplication(argc, argv, programMain);
	
	return 0;
}
