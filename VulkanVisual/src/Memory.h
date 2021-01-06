#pragma once
#include "VulkanFunctionPointers.h"
#include "Device.h"
#include "Swapchain.h"

namespace VulTerGen
{
	void AllocateMemory(Device* device, VkBuffer& buffer, VkDeviceSize bufferSize, VkMemoryPropertyFlags properties, VkDeviceMemory& bufferMemory);
	void CreateVertexBuffer(const float* positions, int size, Device* device, Swapchain* swapchain);
	void DestroyVertexBuffer(Device* device);
	uint32_t findMemoryType(Device* device, uint32_t typeFilter, VkMemoryPropertyFlags properties);
}