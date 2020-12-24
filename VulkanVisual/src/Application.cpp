#include "Application.h"
#include <iostream>
#include <algorithm>
#include <chrono>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace VulTerGen
{
	std::vector<const char*> instanceExtensions = 
	{ 
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME, 
		VK_KHR_SURFACE_EXTENSION_NAME, 
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME 
	};

	std::vector<const char*> layers = 
	{ 
		"VK_LAYER_KHRONOS_validation" 
	};

	std::vector<const char*> deviceExtensions = 
	{ 
		VK_KHR_SWAPCHAIN_EXTENSION_NAME 
	};

	Application::Application(HWND hWnd)
	{
		this->hWnd = hWnd;
		LoadVulkanLibrary();
		CreateVulkanInstance();
		LoadInstanceLevelFunctions(instance);
#ifdef _DEBUG
		InitializeDebug();
#endif

		VkPhysicalDevice selectedDevice = SelectPhysicalDevice();
		device = new Device(selectedDevice, layers, deviceExtensions);

		LoadDeviceLevelFunctions(device->logicalDevice);

		swapchain = new Swapchain(instance, device, hWnd);
		renderPass = new RenderPass(device, swapchain);

		//From Memory
		Memory->CreateVertexBuffer();
		swapchain->CreateFramebuffer(renderPass->renderPass);
		pipeline = new Pipeline(device, swapchain, renderPass);
		command = new Command(device, swapchain, pipeline, renderPass);
	}

	Application::~Application()
	{
		delete command;
		delete pipeline;
		swapchain->DestroyFramebuffer();

		//From Memory
		Memory->DestroyVertexBuffer();
		delete renderPass;
		delete swapchain;
		delete device;

#ifdef _DEBUG
		DeinitializeDebug();
#endif
		DestroyVulkanInstance();
		FreeVulkanLibrary();
	}


	void Application::SetupDraw()
	{
		VkFenceCreateInfo fenceCreateInfo =
		{
			fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			fenceCreateInfo.pNext = nullptr,
			fenceCreateInfo.flags = 0
		};

		vkCreateFence(device->logicalDevice, &fenceCreateInfo, nullptr, &commandBufferExecutionFinished);
		swapchain->SetupDraw(command->commandBuffers);
		time = 0.0f;
		
		//pipeline->ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, -18.0f));
		//pipeline->ubo.view = glm::mat4(1.0f);
		//pipeline->ubo.proj = glm::perspective(glm::radians(45.0f), swapchain->surfaceCapabilities.currentExtent.width / (float)swapchain->surfaceCapabilities.currentExtent.height, 0.1f, 30.0f);

		//void* data;
		//for (size_t i = 0; i < pipeline->uniformBuffersMemory.size(); i++)
		//{
		//	vkMapMemory(device->logicalDevice, pipeline->uniformBuffersMemory[i], 0, sizeof(pipeline->ubo), 0, &data);
		//	memcpy(data, &pipeline->ubo, sizeof(pipeline->ubo));
		//	vkUnmapMemory(device->logicalDevice, pipeline->uniformBuffersMemory[i]);
		//}
	}

	void Application::Draw()
	{
		auto start = std::chrono::high_resolution_clock::now();
		

		//void* data;
		//for (size_t i = 0; i < swapchain->swapchainImages.size(); i++)
		//{
		//	pipeline->ubo.model = glm::rotate(pipeline->ubo.model, glm::radians(0.01f), glm::vec3(0.0f, 1.0f, 0.0f));
		//	vkMapMemory(device->logicalDevice, pipeline->uniformBuffersMemory[i], 0, sizeof(pipeline->ubo), 0, &data);
		//	memcpy(data, &pipeline->ubo, sizeof(pipeline->ubo));
		//	vkUnmapMemory(device->logicalDevice, pipeline->uniformBuffersMemory[i]);
		//}
		
		
		color[0] = glm::abs(glm::sin(time * 4));

		command->RecordCommandBuffer(vertexBuffer, color, time);
		
		vkResetFences(device->logicalDevice, 1, &commandBufferExecutionFinished);
		swapchain->Draw(commandBufferExecutionFinished);
		vkWaitForFences(device->logicalDevice, 1, &commandBufferExecutionFinished, VK_TRUE, UINT64_MAX);

		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> duration = end - start;
		time += duration.count();

	}

	void Application::EndDraw()
	{
		swapchain->EndDraw();
	}

	void Application::CreateVulkanInstance()
	{
		//Desired instance extensions
		assert(CompareDesiredInstanceExtensionsWithAvailable(instanceExtensions) == true);

		//Desired layers
		assert(CompareDesiredLayersWithAvailable(layers) == true);

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
		bool found = false;
		for (auto& desiredExtension : desiredExtensions)
		{
			found = false;
			for (auto& availableExtension : availableExtensions)
			{
				if (!strcmp(availableExtension.extensionName, desiredExtension))
				{
					found = true;
					break;
				}
			}
			//If found proceed to next element
			if (found == true)
			{
				continue;
			}
			//If one layer is not found then exit the loop
			else
			{
				break;
			}
		}
		return found;
	}

	bool Application::CompareDesiredLayersWithAvailable(std::vector<const char*> desiredLayers)
	{
		std::vector<VkLayerProperties> availableLayers = GetAvailableLayers();
		bool found = false;
		for (auto& desiredLayer : desiredLayers)
		{
			found = false;
			for (auto& availableLayer : availableLayers)
			{
				if (!strcmp(availableLayer.layerName, desiredLayer))
				{
					found = true;
					break;
				}
			}
			//If found proceed to next element
			if (found == true)
			{
				continue;
			}
			//If one layer is not found then exit the loop
			else
			{
				break;
			}
		}
		return found;
	}

	bool Application::CompareDesiredDeviceExtensionsWithAvailable(VkPhysicalDevice physicalDevice, std::vector<const char*> desiredDeviceExtensions)
	{
		//std::vector<VkExtensionProperties> availableDeviceExtensions = GetAvailableDeviceExtensions(physicalDevice);
		//std::vector<const char*> availableDeviceExtensionNames(availableDeviceExtensions.size());

		//for (uint32_t i = 0; i < availableDeviceExtensions.size(); ++i)
		//{
		//	availableDeviceExtensionNames[i] = availableDeviceExtensions[i].extensionName;
		//}

		//for (uint32_t i = 0; i < desiredDeviceExtensions.size(); ++i)
		//{
		//	auto found = std::find(std::begin(availableDeviceExtensionNames), std::end(availableDeviceExtensionNames), desiredDeviceExtensions[i]);
		//	if (found != std::end(availableDeviceExtensionNames))
		//	{
		//		//Element found
		//	}
		//	else
		//	{
		//		//Element not found
		//		return false;
		//	}
		//}
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
