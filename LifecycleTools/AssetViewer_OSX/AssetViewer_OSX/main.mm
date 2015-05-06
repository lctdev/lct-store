//
//  main.m
//  SpriteViewer_OSX
//
//  Created by Michael Kron on 3/4/15.
//  Copyright (c) 2015 Michael Kron. All rights reserved.
//

#include <assetViewerProgram.h>

#include <stdio.h>
#include <string.h>

int main(int argc, const char * argv[]) {
    AssetViewerProgram* pProgram = new AssetViewerProgram();
    pProgram->Init();
    pProgram->Run();
    delete pProgram;
    
    return 0;
}
