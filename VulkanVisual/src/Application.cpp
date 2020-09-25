#include "Application.h"
#include <iostream>
#include <algorithm>

namespace VulTerGen
{
	std::vector<const char*> instanceExtensions = { VK_EXT_DEBUG_REPORT_EXTENSION_NAME, VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME };
	std::vector<const char*> layers = { "VK_LAYER_KHRONOS_validation" };
	std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	Application::Application(HWND hWnd)
	{
		this->hWnd = hWnd;
		LoadVulkanLibrary();
		CreateVulkanInstance();
		LoadInstanceLevelFunctions(instance);
#ifdef _DEBUG
		InitializeDebug();
#endif
		if (device == nullptr)
		{
			VkPhysicalDevice selectedDevice = SelectPhysicalDevice();
			device = new Device(selectedDevice, layers, deviceExtensions);

			LoadDeviceLevelFunctions(device->logicalDevice);

			if (swapchain == nullptr)
			{
				swapchain = new Swapchain(instance, device, hWnd);

				if (pipeline == nullptr)
				{
					pipeline = new Pipeline(device->logicalDevice, swapchain->surfaceFormat, swapchain->surfaceCapabilities);

					swapchain->CreateFramebuffer(pipeline->renderPass->renderPass);
					command = new Command(device, swapchain, pipeline);
				}
			}
		}
	}

	Application::~Application()
	{
		if (device != nullptr)
		{
			if (swapchain != nullptr)
			{
				if (pipeline != nullptr)
				{
					delete command;
					swapchain->DestroyFramebuffer();
					delete pipeline;
				}
				delete swapchain;
			}
			delete device;
		}

#ifdef _DEBUG
		DeinitializeDebug();
#endif
		DestroyVulkanInstance();
		FreeVulkanLibrary();
	}

	void Application::SetupDraw()
	{
		swapchain->SetupDraw(command->commandBuffers);
	}

	void Application::Draw()
	{
		swapchain->Draw();
	}

	void Application::EndDraw()
	{
		swapchain->EndDraw();
	}

	void Application::CreateVulkanInstance()
	{
		//Desired instance extensions
		assert(CompareDesiredInstanceExtensionsWithAvailable(instanceExtensions) != true);

		//Desired layers
		assert(CompareDesiredLayersWithAvailable(layers) != true);

		VkApplicationInfo applicationInfo =
		{
			applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			applicationInfo.pNext = nullptr,
			applicationInfo.pApplicationName = "VulTerGen",
			applicationInfo.applicationVersion = VK_MAKE_VERSION(0,0,1),
			applicationInfo.pEngineName = nullptr,
			applicationInfo.engineVersion = 1,
			applicationInfo.apiVersion = VK_API_VERSION_1_2
		};

		VkInstanceCreateInfo instanceInfo =
		{
			instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			// IF DEBUG ENABLED
#ifdef _DEBUG
			instanceInfo.pNext = &debugInfo,
#elif
			instanceInfo.pNext = nullptr,
#endif
			instanceInfo.flags = 0,
			instanceInfo.pApplicationInfo = &applicationInfo,
			instanceInfo.enabledLayerCount = layers.size(),
			instanceInfo.ppEnabledLayerNames = layers.data(),
			instanceInfo.enabledExtensionCount = instanceExtensions.size(),
			instanceInfo.ppEnabledExtensionNames = instanceExtensions.data()
		};

		vkCreateInstance(&instanceInfo, nullptr, &instance);
	}

	void Application::DestroyVulkanInstance()
	{
		vkDestroyInstance(instance, nullptr);
		instance = VK_NULL_HANDLE;
	}

	std::vector<VkExtensionProperties> Application::GetAvailableInstanceExtensions()
	{
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());
		return availableExtensions;
	}

	std::vector<VkLayerProperties> Application::GetAvailableLayers()
	{
		uint32_t layerCount = 0;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
		return availableLayers;
	}

	std::vector<VkExtensionProperties> Application::GetAvailableDeviceExtensions(VkPhysicalDevice physicalDevice)
	{
		uint32_t extensionCount = 0;
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> availableDeviceExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableDeviceExtensions.data());
		return availableDeviceExtensions;
	}

	bool Application::CompareDesiredInstanceExtensionsWithAvailable(std::vector<const char*> desiredExtensions)
	{
		std::vector<VkExtensionProperties> availableExtensions = GetAvailableInstanceExtensions();
		std::vector<const char*> availableExtensionNames(availableExtensions.size());

		for (uint32_t i = 0; i < availableExtensions.size(); ++i)
		{
			availableExtensionNames[i] = availableExtensions[i].extensionName;
		}

		for (uint32_t i = 0; i < desiredExtensions.size(); ++i)
		{
			if (std::find(availableExtensionNames.begin(), availableExtensionNames.end(), desiredExtensions[i]) != availableExtensionNames.end())
			{
				//Element found
			}
			else
			{
				//Element not found
				return false;
			}
		}
	}

	bool Application::CompareDesiredLayersWithAvailable(std::vector<const char*> desiredLayers)
	{
		std::vector<VkLayerProperties> availableLayers = GetAvailableLayers();
		std::vector<const char*> availableLayerNames(availableLayers.size());

		for (uint32_t i = 0; i < availableLayers.size(); ++i)
		{
			availableLayerNames[i] = availableLayers[i].layerName;
		}

		for (uint32_t i = 0; i < desiredLayers.size(); ++i)
		{
			if (std::find(availableLayerNames.begin(), availableLayerNames.end(), desiredLayers[i]) != availableLayerNames.end())
			{
				//Element found
			}
			else
			{
				//Element not found
				return false;
			}
		}
		return true;
	}

	bool Application::CompareDesiredDeviceExtensionsWithAvailable(VkPhysicalDevice physicalDevice, std::vector<const char*> desiredDeviceExtensions)
	{
		std::vector<VkExtensionProperties> availableDeviceExtensions = GetAvailableDeviceExtensions(physicalDevice);
		std::vector<const char*> availableDeviceExtensionNames(availableDeviceExtensions.size());

		for (uint32_t i = 0; i < availableDeviceExtensions.size(); ++i)
		{
			availableDeviceExtensionNames[i] = availableDeviceExtensions[i].extensionName;
		}

		for (uint32_t i = 0; i < desiredDeviceExtensions.size(); ++i)
		{
			auto found = std::find(std::begin(availableDeviceExtensionNames), std::end(availableDeviceExtensionNames), desiredDeviceExtensions[i]);
			if (found != std::end(availableDeviceExtensionNames))
			{
				//Element found
			}
			else
			{
				//Element not found
				return false;
			}
		}
		return true;
	}

	std::vector<VkPhysicalDevice> Application::GetPhysicalDevices()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		std::vector<VkPhysicalDevice> availablePhysicalDevices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, availablePhysicalDevices.data());
		return availablePhysicalDevices;
	}

	void Application::SetPhysicalDeviceFeatures(VkPhysicalDevice physicalDevice)
	{
		uint32_t featureCount = 0;
		VkPhysicalDeviceFeatures desiredDeviceFeatures;
		vkGetPhysicalDeviceFeatures(physicalDevice, &desiredDeviceFeatures);
	}

	void Application::GetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice)
	{
		VkPhysicalDeviceProperties deviceProperties = {};
		vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
	}

	VkPhysicalDevice Application::SelectPhysicalDevice()
	{
		//TODO Workout the logic of getting the physical device
		//Desired device extensions
		std::vector<VkPhysicalDevice> physicalDevices = GetPhysicalDevices();
		VkPhysicalDevice physicalDevice = physicalDevices[0];
		//if (CompareDesiredDeviceExtensionsWithAvailable(physicalDevice, deviceExtensions) == true)
			return physicalDevice;
		return nullptr;
	}

#ifdef _DEBUG
	VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallbackFunction(
		VkDebugReportFlagsEXT                       flags,
		VkDebugReportObjectTypeEXT                  objectType,
		uint64_t                                    object,
		size_t                                      location,
		int32_t                                     messageCode,
		const char* pLayerPrefix,
		const char* pMessage,
		void* pUserData)
	{
		switch (flags)
		{
		case VK_DEBUG_REPORT_ERROR_BIT_EXT:
			std::cout << "ERROR: " << "[" << pLayerPrefix << "] " << pMessage << std::endl;
			break;
		case VK_DEBUG_REPORT_WARNING_BIT_EXT:
			std::cout << "WARNING: " << "[" << pLayerPrefix << "] " << pMessage << std::endl;
			break;
		case VK_DEBUG_REPORT_DEBUG_BIT_EXT:
			std::cout << "DEBUG: " << "[" << pLayerPrefix << "] " << pMessage << std::endl;
			break;
		case VK_DEBUG_REPORT_INFORMATION_BIT_EXT:
			std::cout << "INFORMATION: " << "[" << pLayerPrefix << "] " << pMessage << std::endl;
			break;
		case VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT:
			std::cout << "PERFORMANCE: " << "[" << pLayerPrefix << "] " << pMessage << std::endl;
			break;
		default:
			return true;
			break;
		}
		return false;
	}

	void Application::InitializeDebug()
	{
		vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");

		debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		debugInfo.flags = VK_DEBUG_REPORT_INFORMATION_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT |
			VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_DEBUG_BIT_EXT;
		debugInfo.pfnCallback = DebugCallbackFunction;

		vkCreateDebugReportCallbackEXT(instance, &debugInfo, nullptr, &debugReportCallback);
	}

	void Application::DeinitializeDebug()
	{
		vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
		vkDestroyDebugReportCallbackEXT(instance, debugReportCallback, nullptr);
	}
#endif
}
