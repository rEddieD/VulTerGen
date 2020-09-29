#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>
#include "VulkanFunctionPointers.h"
#include "Device.h"
#include "Swapchain.h"
<<<<<<< HEAD
#include "RenderPass.h"
=======
>>>>>>> origin/master
#include "Pipeline.h"

namespace VulTerGen
{
	class Command
	{
		VkCommandPool commandPool;

		Device* device;
		Swapchain* swapchain;
		Pipeline* pipeline;
<<<<<<< HEAD
		RenderPass* renderPass;

	public:
		Command(Device* device, Swapchain* swapchain, Pipeline* pipeline, RenderPass* renderPass);
=======


		VkDevice logicalDevice;
		uint32_t queueFamilyIndex;
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		VkRenderPass renderPass;
		std::vector<VkFramebuffer> swapchainFramebuffers;
		VkBuffer* vertexBuffers;
		VkPipeline graphicPipeline;

	public:
		Command(Device* device, Swapchain* swapchain, Pipeline* pipeline);
>>>>>>> origin/master
		~Command();

		void CreateCommandPool();
		void DestroyCommandPool();
		void AllocateCommandBufferFromPool();
		void DeallocateCommandBuffersFromPool();
<<<<<<< HEAD

		void RecordCommandBuffer(VkBuffer vertexBuffer);

		std::vector<VkCommandBuffer> commandBuffers;
=======
		void RecordCommandBuffer();

		std::vector<VkCommandBuffer> commandBuffers;

		//VertexBuffer
		void CreateVertexBuffer();
		void DestroyVertexBuffer();
		VkDeviceMemory memoryObject;
		VkBuffer vertexBuffer;
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
>>>>>>> origin/master
	};
}