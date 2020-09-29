#include "Application.h"

namespace VulTerGen
{
	//Exported functions:
	__declspec(dllexport) void Start(HWND hWnd);
	__declspec(dllexport) void Loop();
	__declspec(dllexport) void Stop();

	Application* app;
	bool finished;

	void Start(HWND hWnd)
	{
		finished = false;
		app = new Application(hWnd);
		app->SetupDraw();
		finished = true;
	}

	void Loop()
	{
		if (finished == true)
		{
			app->Draw();
		}
	}

	void Stop()
	{
		app->EndDraw();
		delete app;
	}
}