#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>
#include "VulkanFunctionPointers.h"
#include "Device.h"
#include "Swapchain.h"
#include "RenderPass.h"
#include "Pipeline.h"

namespace VulTerGen
{
	class Command
	{
		VkCommandPool commandPool;

		Device* device;
		Swapchain* swapchain;
		Pipeline* pipeline;
		RenderPass* renderPass;

	public:
		Command(Device* device, Swapchain* swapchain, Pipeline* pipeline, RenderPass* renderPass);
		~Command();

		void CreateCommandPool();
		void DestroyCommandPool();
		void AllocateCommandBufferFromPool();
		void DeallocateCommandBuffersFromPool();

		void RecordCommandBuffer(VkBuffer vertexBuffer, std::array<float, 4> color, float time);

		std::vector<VkCommandBuffer> commandBuffers;
	};
}