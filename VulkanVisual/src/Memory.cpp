#include "Memory.h"

namespace VulTerGen 
{
	VkBuffer vertexBuffer;

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

	void CreateVertexBuffer(const float* positions, int size,Device* device, Swapchain* swapchain)
	{
		VkDeviceMemory memoryObject;
		//float position[] = { -0.5f, -0.5f,  0.5f,
		//					   -0.5f,  0.5f,  0.5f,
		//						0.5f, -0.5f,  0.5f,
		//						0.5f,  0.5f,  0.5f };

		//Recreate pipeline if changing shaders
		VkBufferCreateInfo vertexBufferCreateInfo =
		{
			vertexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			vertexBufferCreateInfo.pNext = nullptr,
			vertexBufferCreateInfo.flags = 0,
			vertexBufferCreateInfo.size = size,
			vertexBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			vertexBufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			vertexBufferCreateInfo.queueFamilyIndexCount = 1,
			vertexBufferCreateInfo.pQueueFamilyIndices = &swapchain->graphicQueueFamilyIndex
		};

		//Creation of vertex buffer
		vkCreateBuffer(device->logicalDevice, &vertexBufferCreateInfo, nullptr, &vertexBuffer);

		//Get what the buffer needs
		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(device->logicalDevice, vertexBuffer, &memoryRequirements);

		VkMemoryAllocateInfo memoryAllocateInfo =
		{
			memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			memoryAllocateInfo.pNext = nullptr,
			memoryAllocateInfo.allocationSize = memoryRequirements.size,
			memoryAllocateInfo.memoryTypeIndex = findMemoryType(device, memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
		};

		vkAllocateMemory(device->logicalDevice, &memoryAllocateInfo, nullptr, &memoryObject);

		vkBindBufferMemory(device->logicalDevice, vertexBuffer, memoryObject, 0);

		void* data;
		vkMapMemory(device->logicalDevice, memoryObject, 0, vertexBufferCreateInfo.size, 0, &data);
		memcpy(data, positions, (size_t)vertexBufferCreateInfo.size);
		vkUnmapMemory(device->logicalDevice, memoryObject);
	}

	void DestroyVertexBuffer(Device* device)
	{
		vkDestroyBuffer(device->logicalDevice, vertexBuffer, nullptr);
	}
}