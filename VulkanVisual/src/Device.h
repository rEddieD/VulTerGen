#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>
#include "VulkanFunctionPointers.h"

namespace VulTerGen
{
	class Device
	{
		struct QueueFamily
		{
			uint32_t queueFamilyIndex;
			VkDeviceQueueCreateInfo queueCreateInfo;
			std::vector<float> priorities;
			uint32_t queueCount;
		};
		QueueFamily queuefamily;



		void CreateLogicalDevice();
		void DestroyLogicalDevice();
		void GetGraphicQueueFamilies();
		void GetDeviceQueue(uint32_t queueFamilyIndex, uint32_t queueIndex);

		std::vector<const char*> layers;
		std::vector<const char*> deviceExtensions;

	public:
		Device(VkPhysicalDevice physicalDevice, std::vector<const char*> layers, std::vector<const char*> deviceExtensions);
		~Device();

		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;

		std::vector<QueueFamily> queueFamilies;
		VkQueue mainGraphicQueue;
	};
}