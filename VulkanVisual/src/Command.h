#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>
#include "VulkanFunctionPointers.h"
#include "Device.h"
#include "Swapchain.h"
#include "Pipeline.h"

namespace VulTerGen
{
	class Command
	{
		VkCommandPool commandPool;

		Device* device;
		Swapchain* swapchain;
		Pipeline* pipeline;


		VkDevice logicalDevice;
		uint32_t queueFamilyIndex;
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		VkRenderPass renderPass;
		std::vector<VkFramebuffer> swapchainFramebuffers;
		VkBuffer* vertexBuffers;
		VkPipeline graphicPipeline;

	public:
		Command(Device* device, Swapchain* swapchain, Pipeline* pipeline);
		~Command();

		void CreateCommandPool();
		void DestroyCommandPool();
		void AllocateCommandBufferFromPool();
		void DeallocateCommandBuffersFromPool();
		void RecordCommandBuffer();

		std::vector<VkCommandBuffer> commandBuffers;

		//VertexBuffer
		void CreateVertexBuffer();
		void DestroyVertexBuffer();
		VkDeviceMemory memoryObject;
		VkBuffer vertexBuffer;
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	};
}