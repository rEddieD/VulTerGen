#include "Application.h"
#include "Visualizer.h"

namespace VulTerGen
{
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