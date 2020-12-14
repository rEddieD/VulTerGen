#include "Windows.h"
#include "ApplicationWindow.h"

namespace VulTerGen
{
	__declspec(dllimport) void Start(HWND hWnd);
	__declspec(dllimport) void Stop();
}

int WINAPI WinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPSTR lpCmdLine,_In_ int nShowCmd)
{
	ApplicationWindow w(hInstance);
	w.EnableConsoleWindow();
	HWND hWnd = w.CreateGraphicalWindow();

	w.EnterFullscreen(hWnd, 1980, 1020, 24, 60);
	VulTerGen::Start(hWnd);
	w.MessageLoop();
	VulTerGen::Stop();
	return 0;
}

