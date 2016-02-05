@echo off

set ASSET_COMPILER_PATH=..\AssetViewer\AssetCompiler\AssetCompiler.jar

set MANIFEST_PATH=script\programAssets.xml
set TEMP_DIR_PATH=temp
set PACKAGE_PATH=data\programAssets.bin

call java -jar %ASSET_COMPILER_PATH% -manifest=%MANIFEST_PATH% -temp=%TEMP_DIR_PATH% -package=%PACKAGE_PATH%