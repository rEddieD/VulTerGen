#include "Device.h"

namespace VulTerGen
{
	Device::Device(VkPhysicalDevice physicalDevice, std::vector<const char*> layers, std::vector<const char*> deviceExtensions)
	{
		this->physicalDevice = physicalDevice;
		this->layers = layers;
		this->deviceExtensions = deviceExtensions;
		GetGraphicQueueFamilies();
		CreateLogicalDevice();
<<<<<<< HEAD
		GetDeviceQueue(queueFamilies[0].queueFamilyIndex, 5);
=======
		GetDeviceQueue(queueFamilies[0].queueFamilyIndex, 0);
>>>>>>> origin/master
	}

	Device::~Device()
	{
		DestroyLogicalDevice();
		this->physicalDevice = VK_NULL_HANDLE;
	}

	void Device::CreateLogicalDevice()
	{
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfo(queueFamilies.size());

		for (uint32_t i = 0; i < queueFamilies.size(); ++i)
		{
			queueCreateInfo[i] = queueFamilies[i].queueCreateInfo;
		}

		VkDeviceCreateInfo logicalDeviceCreateInfo =
		{
			logicalDeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			logicalDeviceCreateInfo.pNext = nullptr,
			logicalDeviceCreateInfo.flags = 0,
			logicalDeviceCreateInfo.queueCreateInfoCount = queueFamilies.size(),
			logicalDeviceCreateInfo.pQueueCreateInfos = queueCreateInfo.data(),
			logicalDeviceCreateInfo.enabledLayerCount = layers.size(),
			logicalDeviceCreateInfo.ppEnabledLayerNames = layers.data(),
			logicalDeviceCreateInfo.enabledExtensionCount = deviceExtensions.size(),
			logicalDeviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data(),
			logicalDeviceCreateInfo.pEnabledFeatures = nullptr
		};

		vkCreateDevice(physicalDevice, &logicalDeviceCreateInfo, nullptr, &logicalDevice);
	}

	void Device::DestroyLogicalDevice()
	{
		if (logicalDevice != VK_NULL_HANDLE)
		{
			vkDestroyDevice(logicalDevice, nullptr);
			logicalDevice = VK_NULL_HANDLE;
		}
	}

	void Device::GetGraphicQueueFamilies()
	{
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, &queueFamilyProperties[0]);

		for (uint32_t i = 0; i < queueFamilyProperties.size(); ++i)
		{
			
			//Add only those queue families that has graphic support.
			if ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) & VK_QUEUE_GRAPHICS_BIT)
			{
				queuefamily.queueFamilyIndex = i;
				queuefamily.queueCount = queueFamilyProperties[i].queueCount;
				queuefamily.priorities.assign(queueFamilyProperties[i].queueCount, 1.0f);

				VkDeviceQueueCreateInfo queueCreateInfo =
				{
					queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
					queueCreateInfo.pNext = nullptr,
					//These are not those flags like VK_QUEUE_GRAPHICS_BIT
					queueCreateInfo.flags = 0,
					queueCreateInfo.queueFamilyIndex = i,
					queueCreateInfo.queueCount = queueFamilyProperties[i].queueCount,
					queueCreateInfo.pQueuePriorities = &queuefamily.priorities[0]
				};

				queuefamily.queueCreateInfo = queueCreateInfo;
				queueFamilies.push_back(queuefamily);
			}
		}
	}

	void Device::GetDeviceQueue(uint32_t queueFamilyIndex, uint32_t queueIndex)
	{
		vkGetDeviceQueue(logicalDevice, queueFamilyIndex, queueIndex, &mainGraphicQueue);
	}
}