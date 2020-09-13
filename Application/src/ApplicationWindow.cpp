#include "ApplicationWindow.h"
#include <string>

namespace VulTerGen
{
	__declspec(dllimport) void Loop();
}

ApplicationWindow::ApplicationWindow(HINSTANCE hInstance)
{
	ConsoleWindowEnabled = false;
	this->hInstance = hInstance;
	windowWidth = 1280;
	windowHeight = 1024;
}

ApplicationWindow::~ApplicationWindow()
{
	if (ConsoleWindowEnabled)
	{
		DestroyConsoleWindow();
	}
	this->wndHandle = NULL;
}

void ApplicationWindow::EnableConsoleWindow()
{
	CreateDebugConsoleWindow();
	ConsoleWindowEnabled = true;
}

void ApplicationWindow::CreateDebugConsoleWindow()
{
	AllocConsole();

	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, NULL, _IONBF, 1);
	*stdout = *hf_out;
	freopen_s(&hf_out, "CONOUT$", "w", stdout);

	HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
	hCrt = _open_osfhandle((long)handle_in, _O_TEXT);
	FILE* hf_in = _fdopen(hCrt, "r");
	setvbuf(hf_in, NULL, _IONBF, 128);
	*stdin = *hf_in;
	freopen_s(&hf_in, "CONIN$", "r", stdin);
}

void ApplicationWindow::DestroyConsoleWindow()
{
	_fcloseall();
	//FreeConsole();
}

HWND ApplicationWindow::RegisterWindowClass(HINSTANCE hInstance, const WNDPROC& windowProc)
{
	WNDCLASSEXW windowClass = {};

	windowClass.cbSize = sizeof(WNDCLASSEXW);
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = windowProc;
	windowClass.hInstance = hInstance;
	windowClass.hCursor = LoadCursorW(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowClass.lpszClassName = L"VulTerGenWindowClass";

	RECT windowRect = {};
	windowRect.left = 0;
	windowRect.right = LONG(windowWidth);
	windowRect.top = 0;
	windowRect.bottom = LONG(windowHeight);

	DWORD style = WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	DWORD exStyle = WS_EX_APPWINDOW;

	AdjustWindowRectEx(&windowRect, style, FALSE, exStyle);

	RegisterClassEx(&windowClass);

	wndHandle = CreateWindowExW(
		NULL,
		L"VulTerGenWindowClass",
		L"VulTerGen Main Window",
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		hInstance,
		NULL);
	return wndHandle;
}



LRESULT CALLBACK ApplicationWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	void (*myPtr)() = &VulTerGen::Loop;
	switch (uMsg)
	{
	case WM_CLOSE:
		//PostQuitMessage(0);
		//PostMessage(hWnd, WM_CLOSE, 0, 0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		FpsCounter(myPtr, hWnd);
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HWND ApplicationWindow::CreateGraphicalWindow()
{
	return RegisterWindowClass(hInstance, WindowProc);
}

void ApplicationWindow::MessageLoop()
{
	MSG msg;

	//Only loops when there is message
	while (GetMessageW(&msg, wndHandle, NULL, NULL) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	    RedrawWindow(wndHandle, NULL, NULL, RDW_INTERNALPAINT);
	}

	
	//Full processing power
	//while (true)
	//{
	//	PeekMessageW(&msg, NULL, NULL, NULL, PM_REMOVE);

	//	if (msg.message == WM_QUIT)
	//	{
	//		break;
	//	}

	//	TranslateMessage(&msg);
	//	DispatchMessageW(&msg);
	//	RedrawWindow(wndHandle, NULL, NULL, RDW_INTERNALPAINT);
	//}
}

void ApplicationWindow::FpsCounter(void (*thisProc)(void), HWND wndHandle)
{
	LARGE_INTEGER startTime;
	LARGE_INTEGER endTime;
	LARGE_INTEGER elapsedTime;

	LARGE_INTEGER frequency;
	
	//Constant from bootup (Large performance counter) Current value: 10 000 000
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&startTime);

	thisProc();

	QueryPerformanceCounter(&endTime);
	elapsedTime.QuadPart = endTime.QuadPart - startTime.QuadPart;
	//Guard against loss of precision
	//elapsedTime.QuadPart *= 10000000;
	//elapsedTime.QuadPart *= 1 / frequency.QuadPart;
	//std::cout << time << " \xE6s" << std::endl;


	//std::cout << elapsedTime.QuadPart << " elapsed tick" << std::endl;
	double time = elapsedTime.QuadPart * ((double)1 / frequency.QuadPart);
	//std::cout << time << " s" << std::endl;
	double frames = double(1) / time;
	//std::cout << frames << " FPS" << std::endl;

	//char mycharArray[] = "Output on the window";


	//_ecvt_s(mycharArray, strlen(mycharArray), frames, 5, nullptr, nullptr);

	//wchar_t wtext[30];
	//mbstowcs_s(nullptr ,wtext, mycharArray, strlen(mycharArray) + 1);//Plus null
	
	std::wstring frameString = std::to_wstring(frames);
	std::wstring mainName = L"VulTerGen Main Window " + frameString;
	LPCTSTR FPS = mainName.c_str();
	SetWindowText(wndHandle, FPS);

}
