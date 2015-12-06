#!/bin/sh

ASSET_COMPILER_PATH=../AssetViewer/AssetCompiler/AssetCompiler.jar

MANIFEST_PATH=script/programAssets.xml
TEMP_DIR_PATH=temp
PACKAGE_PATH=$TARGET_BUILD_DIR/data/programAssets.bin

echo $PACKAGE_PATH

java -jar $ASSET_COMPILER_PATH -manifest=$MANIFEST_PATH -temp=$TEMP_DIR_PATH -package=$PACKAGE_PATH