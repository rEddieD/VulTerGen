#define VK_USE_PLATFORM_WIN32_KHR
#define VK_NO_PROTOTYPE
#include <iostream>
#include <vulkan/vulkan.hpp>
#include <vector>
#include "Visualizer.h"
#include "Shader.h"
#include <bitset>

namespace VulTerGen
{
	std::vector<const char*> layers = { "VK_LAYER_KHRONOS_validation" };
	std::vector<const char*> instanceExtensions = { VK_EXT_DEBUG_REPORT_EXTENSION_NAME, VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME };
	std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	std::vector<std::vector<float>> priorities;
	VkSemaphore semaphore;

	uint32_t GetTotalInstanceExtensions()
	{
		std::vector<VkExtensionProperties> availableExtensions;
		uint32_t extensionCount = 0;

		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		availableExtensions.resize(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, &availableExtensions[0]);
		for (int i = 0; i < availableExtensions.size(); i++)
		{
			std::cout << availableExtensions[i].extensionName << std::endl;
		}
		return extensionCount;
	}

	uint32_t GetTotalLayers()
	{
		std::vector<VkLayerProperties> availableLayers;
		uint32_t layerCount = 0;

		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		availableLayers.resize(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, &availableLayers[0]);
		for (int i = 0; i < availableLayers.size(); i++)
		{
			std::cout << availableLayers[i].layerName << std::endl;
		}
		return layerCount;
	}


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

	void InitVulkanDebug()
	{
		vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");

		debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		debugInfo.flags = VK_DEBUG_REPORT_INFORMATION_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT |
		VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_DEBUG_BIT_EXT;
		debugInfo.pfnCallback = DebugCallbackFunction;

		vkCreateDebugReportCallbackEXT(instance, &debugInfo, nullptr, &debugReportCallback);
	}

	void DeinitVulkanDebug()
	{
		vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
		vkDestroyDebugReportCallbackEXT(instance, debugReportCallback, nullptr);
	}

	void CreateVulkanInstance()
	{
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
			instanceInfo.pNext = &debugInfo,
			instanceInfo.flags = 0,
			instanceInfo.pApplicationInfo = &applicationInfo,
			instanceInfo.enabledLayerCount = layers.size(),
			instanceInfo.ppEnabledLayerNames = layers.data(),
			instanceInfo.enabledExtensionCount = instanceExtensions.size(),
			instanceInfo.ppEnabledExtensionNames = instanceExtensions.data()
		};

		vkCreateInstance(&instanceInfo, nullptr, &instance);
	}

	void GetPhysicalDevice()
	{
		uint32_t deviceCount = 0;

		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		std::vector<VkPhysicalDevice> availablePhysicalDevices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, &availablePhysicalDevices[0]);
		physicalDevice = availablePhysicalDevices[0];
	}

	void GetPhysicalDeviceProperties()
	{

		VkPhysicalDeviceProperties deviceProperties = {};

		vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
		//std::cout << deviceProperties.apiVersion << "\n" << deviceProperties.deviceName << deviceProperties.deviceType;
	}

	void GetPhysicalDeviceExtensions()
	{
		uint32_t extensionCount = 0;

		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> availableDeviceExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, &availableDeviceExtensions[0]);

		for (int i = 0; i < availableDeviceExtensions.size(); i++)
		{
			std::cout << availableDeviceExtensions[i].extensionName << " - " << availableDeviceExtensions[i].specVersion << std::endl;
		}
	}

	void SetPhysicalDeviceFeatures()
	{
		uint32_t featureCount = 0;

		VkPhysicalDeviceFeatures desiredDeviceFeatures;
		vkGetPhysicalDeviceFeatures(physicalDevice, &desiredDeviceFeatures);

		std::cout << desiredDeviceFeatures.alphaToOne << " " << desiredDeviceFeatures.sparseBinding << " " << desiredDeviceFeatures.fragmentStoresAndAtomics << " "
			<< desiredDeviceFeatures.dualSrcBlend << " " << desiredDeviceFeatures.drawIndirectFirstInstance << " " << desiredDeviceFeatures.fillModeNonSolid;
	}

	void GetPhysicalDeviceQueueFamilies()
	{
		uint32_t queueFamilyCount = 0;

		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, &queueFamilyProperties[0]);

		priorities.resize(queueFamilyCount);

		for (uint32_t i = 0; i < queueFamilyProperties.size(); ++i)
		{
			//Add only those queue families that has graphic support.
			if ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) & VK_QUEUE_GRAPHICS_BIT)
			{
				priorities[i].assign(queueFamilyProperties[i].queueCount, 1.0f);

				VkDeviceQueueCreateInfo queueCreateInfo =
				{
					queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
					queueCreateInfo.pNext = nullptr,
					//These are not those flags like VK_QUEUE_GRAPHICS_BIT
					queueCreateInfo.flags = 0,
					queueCreateInfo.queueFamilyIndex = i,
					queueCreateInfo.queueCount = queueFamilyProperties[i].queueCount,
					queueCreateInfo.pQueuePriorities = priorities[i].data()
				};

				queueCreateInfos.push_back(queueCreateInfo);
			}
		}
	}

	void CreateLogicalDevice()
	{
		VkDeviceCreateInfo logicalDeviceCreateInfo =
		{
			logicalDeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			logicalDeviceCreateInfo.pNext = nullptr,
			logicalDeviceCreateInfo.flags = 0,
			logicalDeviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size(),
			logicalDeviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data(),
			logicalDeviceCreateInfo.enabledLayerCount = layers.size(),
			logicalDeviceCreateInfo.ppEnabledLayerNames = layers.data(),
			logicalDeviceCreateInfo.enabledExtensionCount = deviceExtensions.size(),
			logicalDeviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data(),
			logicalDeviceCreateInfo.pEnabledFeatures = nullptr
		};

		vkCreateDevice(physicalDevice, &logicalDeviceCreateInfo, nullptr, &logicalDevice);
	}

	void DestroyLogicalDevice()
	{
		vkDestroyDevice(logicalDevice, nullptr);
		logicalDevice = VK_NULL_HANDLE;
	}

	void GetDeviceQueue()
	{
		vkGetDeviceQueue(logicalDevice, 0, 5, &mainGraphicQueue);
	}
	
	void DestroyVulkanInstance()
	{
		vkDestroyInstance(instance, nullptr);
		instance = VK_NULL_HANDLE;
	}

	void CreatePresentationSurface(HWND hWnd)
	{
		VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {
			surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
			surfaceCreateInfo.pNext = nullptr,
			surfaceCreateInfo.flags = 0,
			surfaceCreateInfo.hinstance = GetModuleHandle(nullptr),
			surfaceCreateInfo.hwnd = hWnd
		};

		vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, nullptr, &presentationSurface);
	}

	bool ImagePresentationSupported()
	{
		VkBool32 supported = VK_FALSE;
		for (const auto &queueInfo : queueCreateInfos)
		{
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueInfo.queueFamilyIndex, presentationSurface, &supported);
			if (supported == VK_TRUE)
			{
				graphicQueueFamilyIndex = queueInfo.queueFamilyIndex;
				return supported;
			}
		}
		return supported;
	}

	bool IsMailboxPresentationSupported()
	{
		uint32_t presentModeCount = 0;

		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, presentationSurface, &presentModeCount, nullptr);
		std::vector<VkPresentModeKHR> presentationModes(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, presentationSurface, &presentModeCount, &presentationModes[0]);

		for (auto& presentationMode : presentationModes)
		{
			if (presentationMode == VkPresentModeKHR::VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return true;
			}
		}
		return false;
	}

	void CreateSwapchain()
	{
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, presentationSurface, &surfaceCapabilities);

		uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, presentationSurface, &formatCount, nullptr);
		std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, presentationSurface, &formatCount, &surfaceFormats[0]);

		VkSwapchainCreateInfoKHR swapchainCreateInfo = {
			swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			swapchainCreateInfo.pNext = nullptr,
			swapchainCreateInfo.flags = 0,
			swapchainCreateInfo.surface = presentationSurface,
			swapchainCreateInfo.minImageCount = surfaceCapabilities.minImageCount,
			//Select the default ones.
			swapchainCreateInfo.imageFormat = surfaceFormats[0].format,
			swapchainCreateInfo.imageColorSpace = surfaceFormats[0].colorSpace,
			swapchainCreateInfo.imageExtent = surfaceCapabilities.currentExtent,
			swapchainCreateInfo.imageArrayLayers = 1,
			swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
			swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
			swapchainCreateInfo.queueFamilyIndexCount = 0,
			swapchainCreateInfo.pQueueFamilyIndices = nullptr,
			swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform,
			swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			swapchainCreateInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR,
			swapchainCreateInfo.clipped = VK_TRUE,
			swapchainCreateInfo.oldSwapchain = nullptr 
		};

		vkCreateSwapchainKHR(logicalDevice, &swapchainCreateInfo, nullptr, &swapchain);
	}

	void GetSwapchainImage()
	{
		uint32_t swapchainImageCount = 0;
		vkGetSwapchainImagesKHR(logicalDevice, swapchain, &swapchainImageCount, nullptr);
		swapchainImages.resize(swapchainImageCount);
		vkGetSwapchainImagesKHR(logicalDevice, swapchain, &swapchainImageCount, &swapchainImages[0]);
	}

	void DestroySwapchain()
	{
		vkDestroySwapchainKHR(logicalDevice, swapchain, nullptr);
	}

	void GetMemoryTypes()
	{
		//Vertex Buffer create info
		VkBufferCreateInfo vertexBufferCreateInfo =
		{
			vertexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			vertexBufferCreateInfo.pNext = nullptr,
			vertexBufferCreateInfo.flags = 0,
			vertexBufferCreateInfo.size = 0,
			vertexBufferCreateInfo.usage = 0,
			vertexBufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			vertexBufferCreateInfo.queueFamilyIndexCount = 0,
			vertexBufferCreateInfo.pQueueFamilyIndices = nullptr
		};

		VkBuffer vertexBuffer;

		//Creation of vertex buffer
		vkCreateBuffer(logicalDevice, &vertexBufferCreateInfo, nullptr, &vertexBuffer);

		//Get the properties of the physical device
		VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &physicalDeviceMemoryProperties);

		//Get what the buffer needs
		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(logicalDevice, vertexBuffer, &memoryRequirements);

		VkDeviceMemory memoryObject;
		VkMemoryPropertyFlagBits memoryProperties = VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

		for (uint32_t type = 0; type < physicalDeviceMemoryProperties.memoryTypeCount; ++type)
		{
			//Check whether this type of memory can be used for the particular buffer
			if (memoryRequirements.memoryTypeBits & (1 << type) && 
				(physicalDeviceMemoryProperties.memoryTypes[type].propertyFlags & memoryProperties) == memoryProperties)
			{
				VkMemoryAllocateInfo memoryAllocateInfo =
				{
					memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
					memoryAllocateInfo.pNext = nullptr,
					memoryAllocateInfo.allocationSize = memoryRequirements.size,
					memoryAllocateInfo.memoryTypeIndex = 0
				};

				vkAllocateMemory(logicalDevice, &memoryAllocateInfo, nullptr, &memoryObject);

				vkBindBufferMemory(logicalDevice, vertexBuffer, memoryObject, 0);
			}
		}

		std::cout << physicalDeviceMemoryProperties.memoryTypes->propertyFlags << std::endl;
		std::cout << physicalDeviceMemoryProperties.memoryTypes->heapIndex << std::endl;
		std::cout << physicalDeviceMemoryProperties.memoryTypeCount << std::endl;
		std::cout << physicalDeviceMemoryProperties.memoryHeaps->flags << std::endl;
		std::cout << physicalDeviceMemoryProperties.memoryHeaps->size << std::endl;
		std::cout << physicalDeviceMemoryProperties.memoryHeapCount << std::endl;
	}

	VkSemaphore& CreateSemaphores()
	{
		VkSemaphore semaphore;
		VkSemaphoreCreateInfo semaphoreCreateInfo =
		{
			semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
			semaphoreCreateInfo.pNext = nullptr,
			semaphoreCreateInfo.flags = 0
		};

		vkCreateSemaphore(logicalDevice, &semaphoreCreateInfo, nullptr, &semaphore);
		return semaphore;
	}

	void DestroySemaphores(VkSemaphore semaphore)
	{
		vkDestroySemaphore(logicalDevice, semaphore, nullptr);
		semaphore = VK_NULL_HANDLE;
	}

	void SetupRenderLoop()
	{
		VkCommandPoolCreateInfo commandPoolCreateInfo =
		{
			commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			commandPoolCreateInfo.pNext = nullptr,
			commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			commandPoolCreateInfo.queueFamilyIndex = graphicQueueFamilyIndex
		};

		VkCommandPool commandPool;

		vkCreateCommandPool(logicalDevice, &commandPoolCreateInfo, nullptr, &commandPool);

		//Allocating command buffer from command pool;
		VkCommandBufferAllocateInfo commandBufferAllocateInfo =
		{
			commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			commandBufferAllocateInfo.pNext = nullptr,
			commandBufferAllocateInfo.commandPool = commandPool,
			commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			commandBufferAllocateInfo.commandBufferCount = 1
		};

		//VkCommandBuffer commandBuffer;

		vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, &commandBuffer);

		//Begin recording into command buffer;
		VkCommandBufferBeginInfo commandBufferBeginInfo =
		{
			commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			commandBufferBeginInfo.pNext = nullptr,
			commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT,
			commandBufferBeginInfo.pInheritanceInfo = nullptr
		};

		VkClearColorValue clearColorValue = { 0.6f, 0.1f, 0.1f, 0.0f };

		vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
		//------------------------------------------------------------

		for (size_t i = 0; i < swapchainImages.size(); i++)
		{
			VkImageSubresourceRange subResourceRange =
			{
				subResourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				subResourceRange.baseMipLevel = 0,
				subResourceRange.levelCount = 1,
				subResourceRange.baseArrayLayer = 0,
				subResourceRange.layerCount = 1
			};

			VkImageMemoryBarrier imageMemoryBarrierPresentToClear =
			{
				imageMemoryBarrierPresentToClear.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
				imageMemoryBarrierPresentToClear.pNext = nullptr,
				imageMemoryBarrierPresentToClear.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT,
				imageMemoryBarrierPresentToClear.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
				imageMemoryBarrierPresentToClear.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
				imageMemoryBarrierPresentToClear.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				imageMemoryBarrierPresentToClear.srcQueueFamilyIndex = graphicQueueFamilyIndex,
				imageMemoryBarrierPresentToClear.dstQueueFamilyIndex = graphicQueueFamilyIndex,
				imageMemoryBarrierPresentToClear.image = swapchainImages[i],
				imageMemoryBarrierPresentToClear.subresourceRange = subResourceRange
			};

			VkImageMemoryBarrier imageMemoryBarrierClearToPresent =
			{
				imageMemoryBarrierClearToPresent.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
				imageMemoryBarrierClearToPresent.pNext = nullptr,
				imageMemoryBarrierClearToPresent.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
				imageMemoryBarrierClearToPresent.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT,
				imageMemoryBarrierClearToPresent.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				imageMemoryBarrierClearToPresent.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
				imageMemoryBarrierClearToPresent.srcQueueFamilyIndex = graphicQueueFamilyIndex,
				imageMemoryBarrierClearToPresent.dstQueueFamilyIndex = graphicQueueFamilyIndex,
				imageMemoryBarrierClearToPresent.image = swapchainImages[i],
				imageMemoryBarrierClearToPresent.subresourceRange = subResourceRange
			};

			vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrierPresentToClear);

			vkCmdClearColorImage(commandBuffer, swapchainImages[i], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearColorValue, 1, &subResourceRange);

			vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrierClearToPresent);
		}

		vkEndCommandBuffer(commandBuffer);

		//Setup renderloop
		//uint32_t swapchainImageIndex;
		/*VkSemaphore */imageAvailableSemaphore = CreateSemaphores();

		/*VkSemaphore */commandBufferFinishedSemaphore = CreateSemaphores();
		/*VkPipelineStageFlags*/ flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		/*VkSubmitInfo */submitInfo =
		{
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			submitInfo.pNext = nullptr,
			submitInfo.waitSemaphoreCount = 1,
			submitInfo.pWaitSemaphores = &imageAvailableSemaphore,
			submitInfo.pWaitDstStageMask = &flags,
			submitInfo.commandBufferCount = 1,
			submitInfo.pCommandBuffers = &commandBuffer,
			submitInfo.signalSemaphoreCount = 1,
			submitInfo.pSignalSemaphores = &commandBufferFinishedSemaphore
		};

		/*VkPresentInfoKHR*/ presentInfo =
		{
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			presentInfo.pNext = nullptr,
			presentInfo.waitSemaphoreCount = 1,
			presentInfo.pWaitSemaphores = &commandBufferFinishedSemaphore,
			presentInfo.swapchainCount = 1,
			presentInfo.pSwapchains = &swapchain,
			presentInfo.pImageIndices = &swapchainImageIndex,
			presentInfo.pResults = nullptr
		};
	}

	void RenderLoop()
	{
		vkAcquireNextImageKHR(logicalDevice, swapchain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &swapchainImageIndex);

		//Wait previous actions
		vkQueueWaitIdle(mainGraphicQueue);

		vkQueueSubmit(mainGraphicQueue, 1, &submitInfo, nullptr);

		vkQueuePresentKHR(mainGraphicQueue, &presentInfo);	
	}

	void DestroyRenderLoop()
	{
		//Destroy renderloop
		vkDeviceWaitIdle(logicalDevice);

		DestroySemaphores(commandBufferFinishedSemaphore);
		DestroySemaphores(imageAvailableSemaphore);
	}


	void CreatePipeline()
	{
		//Recreate pipeline if changing shaders
		float positions[] = { -0.5f, -0.5f,  0.5f,
							   0.5f, -0.5f,  0.5f,
							   0.5f,  0.5f,  0.5f,
							  -0.5f,  0.5f,  0.5f };

		unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

		//Single module can contain code for multiple shader stages
		//Shader compilation and linkage is performed during the pipeline object creation.
		VkShaderModule vertexShaderModule = nullptr;
		VkShaderModule fragmentShaderModule = nullptr;

		//Shader module
		VkShaderModuleCreateInfo vertexShaderModuleCreateInfo = {
			vertexShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			vertexShaderModuleCreateInfo.pNext = nullptr,
			vertexShaderModuleCreateInfo.flags = 0,
			vertexShaderModuleCreateInfo.codeSize = 0, //have to change
			vertexShaderModuleCreateInfo.pCode = nullptr //have to change
		};

		VkShaderModuleCreateInfo fragmentShaderModuleCreateInfo = {
			fragmentShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			fragmentShaderModuleCreateInfo.pNext = nullptr,
			fragmentShaderModuleCreateInfo.flags = 0,
			fragmentShaderModuleCreateInfo.codeSize = 0, //have to change
			fragmentShaderModuleCreateInfo.pCode = nullptr //have to change
		};

		//vkCreateShaderModule(logicalDevice, &vertexShaderModuleCreateInfo, nullptr, &vertexShaderModule);
		//vkCreateShaderModule(logicalDevice, &fragmentShaderModuleCreateInfo, nullptr, &fragmentShaderModule);

		VkPipelineShaderStageCreateInfo vertexShaderStageCreateInfo = {
			vertexShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			vertexShaderStageCreateInfo.pNext = nullptr,
			vertexShaderStageCreateInfo.flags = 0,
			vertexShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT,
			vertexShaderStageCreateInfo.module = vertexShaderModule,
			vertexShaderStageCreateInfo.pName = "main",
			vertexShaderStageCreateInfo.pSpecializationInfo = nullptr
		};

		VkPipelineShaderStageCreateInfo fragmentShaderStageCreateInfo = {
			fragmentShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			fragmentShaderStageCreateInfo.pNext = nullptr,
			fragmentShaderStageCreateInfo.flags = 0,
			fragmentShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
			fragmentShaderStageCreateInfo.module = fragmentShaderModule,
			fragmentShaderStageCreateInfo.pName = "main",
			fragmentShaderStageCreateInfo.pSpecializationInfo = nullptr
		};




		VkVertexInputBindingDescription vertexInputBindingDescription = {
			vertexInputBindingDescription.binding = 0, //where the data is taken from (from which binding)
			vertexInputBindingDescription.stride = 6 * sizeof(float), //what is the stride between consecutive elements in the buffer pos.x, pos.y, pos.z, col.x, col.y, col.z
			vertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX //how this data is read
		};

		std::vector<VkVertexInputAttributeDescription> vertexInputAttribDescriptions(2);

		//These are the vertex position coordinates
		VkVertexInputAttributeDescription vertexInputPosAttributeDescription = {
			vertexInputPosAttributeDescription.location = 0,
			vertexInputPosAttributeDescription.binding = 0,
			vertexInputPosAttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT,
			vertexInputPosAttributeDescription.offset = 0
		};

		vertexInputAttribDescriptions.push_back(vertexInputPosAttributeDescription);

		VkVertexInputAttributeDescription vertexInputColAttributeDescription = {
			vertexInputColAttributeDescription.location = 1,
			vertexInputColAttributeDescription.binding = 0,
			vertexInputColAttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT,
			vertexInputColAttributeDescription.offset = 3 * sizeof(float)
		};

		vertexInputAttribDescriptions.push_back(vertexInputColAttributeDescription);

		//If it is hardcoded in the shader then no need to specify
		VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo = {
			pipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
			pipelineVertexInputStateCreateInfo.pNext = nullptr,
			pipelineVertexInputStateCreateInfo.flags = 0,
			pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = 0,		//1,
			pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = nullptr,	//&vertexInputBindingDescription,
			pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = 0,		//vertexInputAttribDescriptions.size(),
			pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = nullptr	//vertexInputAttribDescriptions.data()
		};

		VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo = {
			pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			pipelineInputAssemblyStateCreateInfo.pNext = nullptr,
			pipelineInputAssemblyStateCreateInfo.flags = 0,
			pipelineInputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
			pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE
		};

		VkViewport viewport = {
			viewport.x = 0,
			viewport.y = 0,
			viewport.width = surfaceCapabilities.currentExtent.width,
			viewport.height = surfaceCapabilities.currentExtent.height,
			viewport.minDepth = 0.0f,
			viewport.maxDepth = 0.0f
		};

		VkRect2D scissor = {
			scissor.offset = {0, 0},
			scissor.extent = surfaceCapabilities.currentExtent
		};

		VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo = {
			pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			pipelineViewportStateCreateInfo.pNext = nullptr,
			pipelineViewportStateCreateInfo.flags = 0,
			pipelineViewportStateCreateInfo.viewportCount = 1,
			pipelineViewportStateCreateInfo.pViewports = &viewport,
			pipelineViewportStateCreateInfo.scissorCount = 1,
			pipelineViewportStateCreateInfo.pScissors = &scissor
		};


		//VkPipelineRasterizationStateCreateInfo pipelineRasterizerStateCreateInfo = {
		//	pipelineRasterizerStateCreateInfo.sType = ,
		//	pipelineRasterizerStateCreateInfo.pNext = ,
		//	pipelineRasterizerStateCreateInfo.flags = ,
		//	pipelineRasterizerStateCreateInfo.
		//};
	}

	
	void Start(HWND hWnd)
	{
		LoadVulkanLibrary();

		//GetTotalInstanceExtensions();
		//GetTotalLayers();
		
		CreateVulkanInstance();
		InitVulkanDebug();

		LoadInstanceLevelFunctions();

		GetPhysicalDevice();
		//GetPhysicalDeviceProperties();
		//GetPhysicalDeviceExtensions();
		//SetPhysicalDeviceFeatures();
		
		GetPhysicalDeviceQueueFamilies();

		CreatePresentationSurface(hWnd);
		if (ImagePresentationSupported())
		{
			//std::cout << "Mailbox presentaiton supported: " << (IsMailboxPresentationSupported() ? "true" : "false") << std::endl;
			CreateLogicalDevice();

			LoadDeviceLevelFunctions();

			GetDeviceQueue();
			CreateSwapchain();


			//ACQUIRE AN IMAGE FROM THE SWAPCHAIN
			GetSwapchainImage();
			SetupRenderLoop();
			//RenderLoop();
			//---------------------------------

			Shader vertexShader(logicalDevice, "shader\vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
			Shader fragmentShader(logicalDevice, "shader\frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

			//DestroySwapchain();
			//DestroyLogicalDevice();
		}

		
		//DeinitVulkanDebug();
		//DestroyVulkanInstance();

		//FreeVulkanLibrary();
	}

	void Loop()
	{
		RenderLoop();
	}

	void Stop()
	{
		DestroyRenderLoop();
		DestroySwapchain();
		DestroyLogicalDevice();

		DeinitVulkanDebug();
		DestroyVulkanInstance();

		FreeVulkanLibrary();
	}
}

//void EmptyFunction(){}
//void(*fncPtr)() = EmptyFunction;

//typedef long(*function)(void*, const char*);
//function vkGetInstanceProcAddr = (function)GetProcAddress(vulkan_library, "vkGetInstanceProcAddr");
//vkGetInstanceProcAddr(NULL, "vkCreateInstance")

//auto vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)GetProcAddress(vulkan_library, "vkGetInstanceProcAddr");

//int main()
//{
//	HMODULE vulkan_library = LoadLibrary(L"vulkan-1.dll");
//	if (vulkan_library == nullptr)
//	{
//	}
//	else
//	{
//		PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)GetProcAddress(vulkan_library, "vkGetInstanceProcAddr");
//
//		PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties = (PFN_vkEnumerateInstanceExtensionProperties)vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceExtensionProperties");
//		PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties = (PFN_vkEnumerateInstanceLayerProperties)vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceLayerProperties");
//		PFN_vkCreateInstance vkCreateInstance = (PFN_vkCreateInstance)vkGetInstanceProcAddr(nullptr, "vkCreateInstance");
//
//		uint32_t extensionCount = 0;
//
//		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
//		std::cout << extensionCount;
//	}
//	return 0;
//}

//void CreateCommandPool()
//{
//	VkCommandPoolCreateInfo commandPoolCreateInfo =
//	{
//		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
//		commandPoolCreateInfo.pNext = nullptr,
//		commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
//		commandPoolCreateInfo.queueFamilyIndex = graphicQueueFamilyIndex
//	};
//
//	VkCommandPool commandPool;
//
//	vkCreateCommandPool(logicalDevice, &commandPoolCreateInfo, nullptr, &commandPool);
//
//	//Allocating command buffer from command pool;
//	VkCommandBufferAllocateInfo commandBufferAllocateInfo =
//	{
//		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
//		commandBufferAllocateInfo.pNext = nullptr,
//		commandBufferAllocateInfo.commandPool = commandPool,
//		commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
//		commandBufferAllocateInfo.commandBufferCount = 1
//	};
//
//	VkCommandBuffer commandBuffer;
//
//	vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, &commandBuffer);
//
//	//Begin recording into command buffer;
//	VkCommandBufferBeginInfo commandBufferBeginInfo =
//	{
//		commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
//		commandBufferBeginInfo.pNext = nullptr,
//		commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
//		commandBufferBeginInfo.pInheritanceInfo = nullptr
//	};
//
//	VkClearColorValue clearColorValue = { 0.6f, 0.1f, 0.1f, 0.0f };
//
//	vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
//	//------------------------------------------------------------
//
//	for (size_t i = 0; i < swapchainImages.size(); i++)
//	{
//		VkImageSubresourceRange subResourceRange =
//		{
//			subResourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
//			subResourceRange.baseMipLevel = 0,
//			subResourceRange.levelCount = 1,
//			subResourceRange.baseArrayLayer = 0,
//			subResourceRange.layerCount = 1
//		};
//
//		VkImageMemoryBarrier imageMemoryBarrierPresentToClear =
//		{
//			imageMemoryBarrierPresentToClear.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
//			imageMemoryBarrierPresentToClear.pNext = nullptr,
//			imageMemoryBarrierPresentToClear.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT,
//			imageMemoryBarrierPresentToClear.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
//			imageMemoryBarrierPresentToClear.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
//			imageMemoryBarrierPresentToClear.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
//			imageMemoryBarrierPresentToClear.srcQueueFamilyIndex = graphicQueueFamilyIndex,
//			imageMemoryBarrierPresentToClear.dstQueueFamilyIndex = graphicQueueFamilyIndex,
//			imageMemoryBarrierPresentToClear.image = swapchainImages[i],
//			imageMemoryBarrierPresentToClear.subresourceRange = subResourceRange
//		};
//
//		VkImageMemoryBarrier imageMemoryBarrierClearToPresent =
//		{
//			imageMemoryBarrierClearToPresent.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
//			imageMemoryBarrierClearToPresent.pNext = nullptr,
//			imageMemoryBarrierClearToPresent.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
//			imageMemoryBarrierClearToPresent.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT,
//			imageMemoryBarrierClearToPresent.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
//			imageMemoryBarrierClearToPresent.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
//			imageMemoryBarrierClearToPresent.srcQueueFamilyIndex = graphicQueueFamilyIndex,
//			imageMemoryBarrierClearToPresent.dstQueueFamilyIndex = graphicQueueFamilyIndex,
//			imageMemoryBarrierClearToPresent.image = swapchainImages[i],
//			imageMemoryBarrierClearToPresent.subresourceRange = subResourceRange
//		};
//
//		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrierPresentToClear);
//
//		vkCmdClearColorImage(commandBuffer, swapchainImages[i], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearColorValue, 1, &subResourceRange);
//
//		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrierClearToPresent);
//	}
//
//
//
//	//------------------------------------------------------------
//	//End recording into command buffer;
//	vkEndCommandBuffer(commandBuffer);
//
//	//Synchronization objects
//	//Semaphores are used to synchronize submitted command buffers with each other. 
//	//Fences are used to synchronize an application with submitted commands.
//	VkSemaphore semaphoreForCommandBuffer;
//
//	VkSemaphoreCreateInfo semaphoreCreateInfo =
//	{
//		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
//		semaphoreCreateInfo.pNext = nullptr,
//		semaphoreCreateInfo.flags = 0
//	};
//
//	vkCreateSemaphore(logicalDevice, &semaphoreCreateInfo, nullptr, &semaphoreForCommandBuffer);
//
//	/* Creation of fences
//	VkFenceCreateInfo fenceCreateInfo =
//	{
//		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
//		fenceCreateInfo.pNext = nullptr,
//		fenceCreateInfo.flags = 0
//	};
//
//	VkFence fence;
//
//	vkCreateFence(logicalDevice, &fenceCreateInfo, nullptr, &fence);
//
//	//Semaphores automatically reset but it is the application responsibility to reset the fence
//	vkResetFences(logicalDevice, 1, &fence);*/
//
//	//Submit a command buffer to a queue
//	VkPipelineStageFlags flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//
//	VkSubmitInfo submitInfo =
//	{
//		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
//		submitInfo.pNext = nullptr,
//		submitInfo.waitSemaphoreCount = 1,//1,
//		submitInfo.pWaitSemaphores = &semaphore,//&semaphoreForCommandBuffer,
//		submitInfo.pWaitDstStageMask = &flags,
//		submitInfo.commandBufferCount = 1,
//		submitInfo.pCommandBuffers = &commandBuffer,
//		submitInfo.signalSemaphoreCount = 1,
//		submitInfo.pSignalSemaphores = &semaphoreForCommandBuffer
//	};
//
//
//	vkQueueSubmit(mainGraphicQueue, 1, &submitInfo, nullptr);
//
//	//Application wait on fence(s)
//	//vkWaitForFences(logicalDevice, 1, &fence, VK_TRUE, 2000);
//
//	//Wait on the logical device to finish
//	vkDeviceWaitIdle(logicalDevice);
//
//	//When command buffer was previously recorded or there were errors it must be reset before
//	//rerecorded once again
//	//Can be done implicitly with another record operation or do it explicitly like this
//
//	//vkResetCommandBuffer(commandBuffer, 0);
//
//	//vkResetCommandPool(logicalDevice, commandPool, 0);
//
//	vkDestroySemaphore(logicalDevice, semaphoreForCommandBuffer, nullptr);
//	//vkDestroyFence(logicalDevice, fence, nullptr);
//	//No need to explicitly free from command pool, because the command buffer is implicitly deleted with the command pool
//	//vkFreeCommandBuffers(logicalDevice, commandPool, 1, &commandBuffer);
//	vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
//}