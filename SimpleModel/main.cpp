#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "SimpleModel.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef _DEBUG
	::AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONIN$", "r", stdin);
#endif // _DEBUG


	SimpleModel simpleModel;
	simpleModel.start();

#ifdef _DEBUG
	::FreeConsole();
#endif // _DEBUG


	return 0;
}