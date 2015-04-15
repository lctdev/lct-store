#include <windows.h>

#include <assetViewerProgram.h>

AssetViewerProgram program;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	program.SetHInstance(hInstance);
	program.SetNCmdShow(nCmdShow);
	program.Init();

	program.Run();

	return 0;
}
