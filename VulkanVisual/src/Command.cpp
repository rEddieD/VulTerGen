#include "Command.h"

namespace VulTerGen
{
	Command::Command(Device* device, Swapchain* swapchain, Pipeline* pipeline, RenderPass* renderPass)
	{
		this->device = device;
		this->swapchain = swapchain;
		this->pipeline = pipeline;
		this->renderPass = renderPass;
		commandBuffers.resize(swapchain->swapchainFramebuffers.size());
		CreateCommandPool();
		AllocateCommandBufferFromPool();
	}

	Command::~Command()
	{
		//DeallocateCommandBuffersFromPool(); Not needed the command pool implicitly destorys it
		DestroyCommandPool();
	}

	void Command::CreateCommandPool()
	{
		VkCommandPoolCreateInfo commandPoolCreateInfo =
		{
			commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			commandPoolCreateInfo.pNext = nullptr,
			commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			commandPoolCreateInfo.queueFamilyIndex = swapchain->graphicQueueFamilyIndex
		};

		vkCreateCommandPool(device->logicalDevice, &commandPoolCreateInfo, nullptr, &commandPool);
	}

	void Command::DestroyCommandPool()
	{
		if (commandPool != VK_NULL_HANDLE)
		{
			vkDestroyCommandPool(device->logicalDevice, commandPool, nullptr);
			commandPool = VK_NULL_HANDLE;
		}
	}

	void Command::AllocateCommandBufferFromPool()
	{
		VkCommandBufferAllocateInfo commandBufferAllocateInfo =
		{
			commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			commandBufferAllocateInfo.pNext = nullptr,
			commandBufferAllocateInfo.commandPool = commandPool,
			commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			commandBufferAllocateInfo.commandBufferCount = (uint32_t)commandBuffers.size()
		};

		vkAllocateCommandBuffers(device->logicalDevice, &commandBufferAllocateInfo, commandBuffers.data());
	}

	void Command::DeallocateCommandBuffersFromPool()
	{
		if (commandBuffers.size() > 0)
		{
			vkFreeCommandBuffers(device->logicalDevice, commandPool, commandBuffers.size(), commandBuffers.data());
			commandBuffers.clear();
		}
	}

	void Command::RecordCommandBuffer(VkBuffer vertexBuffer)
	{
		VkCommandBufferBeginInfo commandBufferBeginInfo =
		{
			commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			commandBufferBeginInfo.pNext = nullptr,
			commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT,
			commandBufferBeginInfo.pInheritanceInfo = nullptr
		};

		VkClearValue clearColor = { 0.5f, 0.8f, 0.3f, 0.0f };
		VkClearValue clearDepth = { 0.0f, 0 };

		VkClearValue clearValues[2] = { clearColor, clearDepth };

		for (uint32_t i = 0; i < swapchain->swapchainFramebuffers.size(); ++i)
		{
			VkRenderPassBeginInfo renderPassInfo = {
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
				renderPassInfo.pNext = nullptr,
				renderPassInfo.renderPass = renderPass->renderPass,
				renderPassInfo.framebuffer = swapchain->swapchainFramebuffers[i],
				renderPassInfo.renderArea.offset = {0, 0},
				renderPassInfo.renderArea.extent = swapchain->surfaceCapabilities.currentExtent,
				renderPassInfo.clearValueCount = 1,
				renderPassInfo.pClearValues = &clearColor
			};

			vkBeginCommandBuffer(commandBuffers[i], &commandBufferBeginInfo);

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->graphicPipeline);

			VkBuffer vertexBuffers[] = { vertexBuffer };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);

			vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(commandBuffers[i]);

			vkEndCommandBuffer(commandBuffers[i]);
		}
	}
}
