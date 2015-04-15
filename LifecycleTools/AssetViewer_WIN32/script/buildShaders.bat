set SPRITE_SHADER_DIR=..\LifecycleRuntime\shader\spri\WIN32
set FONT_SHADER_DIR=..\LifecycleRuntime\shader\font\WIN32
set FILL_SHADER_DIR=..\LifecycleRuntime\shader\fill\WIN32
set BUILD_DIR=.\data\shad

:: create the directory
if not exist %BUILD_DIR% mkdir %BUILD_DIR%

:: copy all shaders
copy %SPRITE_SHADER_DIR%\*.* %BUILD_DIR%
copy %FONT_SHADER_DIR%\*.* %BUILD_DIR%
copy %FILL_SHADER_DIR%\*.* %BUILD_DIR%