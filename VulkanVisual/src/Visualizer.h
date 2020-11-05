#pragma once
#include <Windows.h>
#include "VulkanFunctionPointers.h"

namespace VulTerGen
{
	//Exported functions:
	__declspec(dllexport) void Start(HWND hWnd);
	__declspec(dllexport) void Loop();
	__declspec(dllexport) void Stop();
}