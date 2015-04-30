#!/bin/sh

SPRITE_SHADER_DIR=../LifecycleRuntime/shader/spri/OSX
FONT_SHADER_DIR=../LifecycleRuntime/shader/font/OSX
FILL_SHADER_DIR=../LifecycleRuntime/shader/fill/OSX
BUILD_DIR=$TARGET_BUILD_DIR/data/shad

# create the directory
if [ ! -d "$BUILD_DIR" ]; then
	mkdir -p $BUILD_DIR
fi

# copy all shaders
cp $SPRITE_SHADER_DIR/*.* $BUILD_DIR
cp $FONT_SHADER_DIR/*.* $BUILD_DIR
cp $FILL_SHADER_DIR/*.* $BUILD_DIR