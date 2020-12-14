#pragma once

#include "Windows.h"
#include <iostream>
#include <io.h>
#include <fcntl.h>

class ApplicationWindow
{
public:
	ApplicationWindow(HINSTANCE hInstance);
	~ApplicationWindow();

	void EnableConsoleWindow();
	HWND CreateGraphicalWindow();
	void MessageLoop();

	bool EnterFullscreen(HWND hwnd, int fullscreenWidth, int fullscreenHeight, int colourBits, int refreshRate);
	bool ExitFullscreen(HWND hwnd, int windowX, int windowY, int windowedWidth, int windowedHeight, int windowedPaddingX, int windowedPaddingY);

private:
	bool ConsoleWindowEnabled;
	HWND wndHandle;

	HINSTANCE hInstance;
	
	int windowWidth;
	int windowHeight;

	void CreateDebugConsoleWindow();
	void DestroyConsoleWindow();

	HWND RegisterWindowClass(HINSTANCE hInstance, const WNDPROC& windowProc);
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static void FpsCounter(void (*thisProc)(void), HWND wndHandle);
};