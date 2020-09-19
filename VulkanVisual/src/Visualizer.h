#pragma once
#include <Windows.h>
#include "VulkanFunctionPointers.h"
//This is what VulkanFunctionPointer contains here
//namespace VulTerGen
//{
//bool LoadVulkanLibrary();
//extern PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
//extern PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
//}


namespace VulTerGen
{
	//Exported functions:
	__declspec(dllexport) void Start(HWND hWnd);
	__declspec(dllexport) void Loop();
	__declspec(dllexport) void Stop();

	class Application
	{
	public:
		Application();
		~Application();
	private:
	};

	//Main Vulkan objects
	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkDevice logicalDevice;

	//Surface related
	VkSurfaceKHR presentationSurface;
	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	VkSurfaceFormatKHR surfaceFormat;

	//Queues
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	uint32_t graphicQueueFamilyIndex;
	VkQueue mainGraphicQueue;

	//Swapchain
	VkSwapchainKHR swapchain;
	std::vector<VkImage> swapchainImages;
	uint32_t swapchainImageIndex;

	VkSemaphore imageAvailableSemaphore;
	VkSemaphore commandBufferFinishedSemaphore;
	VkSubmitInfo submitInfo;
	VkPresentInfoKHR presentInfo;
	VkCommandBuffer commandBuffer;
	VkPipelineStageFlags flags;
	
	//Debug
	PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT;
	PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT;
	VkDebugReportCallbackEXT debugReportCallback;
	VkDebugReportCallbackCreateInfoEXT debugInfo;
}