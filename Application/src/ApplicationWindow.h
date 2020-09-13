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