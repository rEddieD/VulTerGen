#include "Memory.h"

namespace VulTerGen 
{
	uint32_t findMemoryType(Device* device, uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(device->physicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}
	}

	void AllocateMemory(Device* device, VkBuffer &buffer ,VkDeviceSize bufferSize, VkMemoryPropertyFlags properties, VkDeviceMemory& bufferMemory)
	{
		VkBufferCreateInfo bufferInfo = {
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			bufferInfo.pNext = nullptr,
			bufferInfo.flags = 0,
			bufferInfo.size = bufferSize,
			bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			bufferInfo.queueFamilyIndexCount = 0,
			bufferInfo.pQueueFamilyIndices = nullptr
		};

		vkCreateBuffer(device->logicalDevice, &bufferInfo, nullptr, &buffer);

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device->logicalDevice, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(device, memRequirements.memoryTypeBits, properties);

		vkAllocateMemory(device->logicalDevice, &allocInfo, nullptr, &bufferMemory);

		vkBindBufferMemory(device->logicalDevice, buffer, bufferMemory, 0);
	}
}