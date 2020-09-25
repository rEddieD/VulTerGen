#pragma once
#define VK_NO_PROTOTYPE
#define VK_USE_PLATFORM_WIN32_KHR
#include <Windows.h>
#include <vulkan/vulkan.hpp>
#include "VulkanFunctionPointers.h"
#include "Device.h"
#include "Swapchain.h"
#include "Pipeline.h"
#include "Command.h"

namespace VulTerGen
{
	class Application
	{
		VkInstance instance;
		HWND hWnd;

		VkPhysicalDevice physicalDevice;
		Device* device;
		Swapchain* swapchain;

		//All of the physical devices
		//std::vector<VkPhysicalDevice> physicalDevices;
		Pipeline* pipeline;
		Command* command;

	public:
		Application(HWND hWnd);
		~Application();

		void SetupDraw();
		void Draw();
		void EndDraw();

	private:
		void CreateVulkanInstance();
		void DestroyVulkanInstance();

		//Helper functions:
		std::vector<VkExtensionProperties> GetAvailableInstanceExtensions();
		std::vector<VkLayerProperties> GetAvailableLayers();
		std::vector< VkExtensionProperties> GetAvailableDeviceExtensions(VkPhysicalDevice physicalDevice);

		bool CompareDesiredInstanceExtensionsWithAvailable(std::vector<const char*> desiredExtensions);
		bool CompareDesiredLayersWithAvailable(std::vector<const char*> desiredLayers);
		bool CompareDesiredDeviceExtensionsWithAvailable(VkPhysicalDevice physicalDevice, std::vector<const char*> desiredDeviceExtensions);
		
		void SetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice); //Need to implement further
		void GetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice); //Need to implement further

		std::vector<VkPhysicalDevice> GetPhysicalDevices();
		VkPhysicalDevice SelectPhysicalDevice();

#ifdef _DEBUG
		VkDebugReportCallbackCreateInfoEXT debugInfo;
		PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT;
		PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT;
		VkDebugReportCallbackEXT debugReportCallback;

		void InitializeDebug();
		void DeinitializeDebug();
#endif
	};
}