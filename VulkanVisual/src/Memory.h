#pragma once
#include <vulkan/vulkan.hpp>
#include "VulkanFunctionPointers.h"
#include "Device.h"

namespace VulTerGen
{
	void AllocateMemory(Device* device, VkBuffer& buffer, VkDeviceSize bufferSize, VkMemoryPropertyFlags properties, VkDeviceMemory& bufferMemory);
	void CreateVertexBuffer(float positions[]);
	void DestroyVertexBuffer();
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
}