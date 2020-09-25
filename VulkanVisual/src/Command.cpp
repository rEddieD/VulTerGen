#include "Command.h"

namespace VulTerGen
{
	Command::Command(Device* device, Swapchain* swapchain, Pipeline* pipeline)
	{
		this->device = device;
		logicalDevice = device->logicalDevice;
		queueFamilyIndex = swapchain->graphicQueueFamilyIndex;
		surfaceCapabilities = swapchain->surfaceCapabilities;
		renderPass = pipeline->renderPass->renderPass;
		swapchainFramebuffers = swapchain->swapchainFramebuffers;
		graphicPipeline = pipeline->graphicPipeline;
		CreateCommandPool();
		CreateVertexBuffer();
	}

	Command::~Command()
	{
		DestroyVertexBuffer();
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
			commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex
		};

		vkCreateCommandPool(logicalDevice, &commandPoolCreateInfo, nullptr, &commandPool);
	}

	void Command::DestroyCommandPool()
	{
		if (commandPool != VK_NULL_HANDLE)
		{
			vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
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
			commandBufferAllocateInfo.commandBufferCount = commandBuffers.size()
		};

		vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, commandBuffers.data());
	}

	void Command::DeallocateCommandBuffersFromPool()
	{
		if (commandBuffers.size() > 0)
		{
			vkFreeCommandBuffers(logicalDevice, commandPool, commandBuffers.size(), commandBuffers.data());
			commandBuffers.clear();
		}
	}

	void Command::RecordCommandBuffer()
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

		for (uint32_t i = 0; i < swapchainFramebuffers.size(); ++i)
		{
			VkRenderPassBeginInfo renderPassInfo = {
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
				renderPassInfo.pNext = nullptr,
				renderPassInfo.renderPass = renderPass,
				renderPassInfo.framebuffer = swapchainFramebuffers[i],
				renderPassInfo.renderArea.offset = {0, 0},
				renderPassInfo.renderArea.extent = surfaceCapabilities.currentExtent,
				renderPassInfo.clearValueCount = 1,
				renderPassInfo.pClearValues = &clearColor
			};

			vkBeginCommandBuffer(commandBuffers[i], &commandBufferBeginInfo);

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicPipeline);

			vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, 0);

			vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(commandBuffers[i]);

			vkEndCommandBuffer(commandBuffers[i]);
		}
	}

	void Command::CreateVertexBuffer()
	{
		float positions[] = {  -0.5f, -0.8f,  0.0f,
						0.5f, -0.5f,  0.0f,
						0.5f,  0.5f,  0.0f };

		//Recreate pipeline if changing shaders
		VkBufferCreateInfo vertexBufferCreateInfo =
		{
			vertexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			vertexBufferCreateInfo.pNext = nullptr,
			vertexBufferCreateInfo.flags = 0,
			vertexBufferCreateInfo.size = sizeof(positions),
			vertexBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			vertexBufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			vertexBufferCreateInfo.queueFamilyIndexCount = 1,
			vertexBufferCreateInfo.pQueueFamilyIndices = &device->queueFamilies[0].queueFamilyIndex
		};
		
		//Creation of vertex buffer
		vkCreateBuffer(device->logicalDevice, &vertexBufferCreateInfo, nullptr, &vertexBuffer);
		
		//Get what the buffer needs
		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(logicalDevice, vertexBuffer, &memoryRequirements);
		
		VkMemoryAllocateInfo memoryAllocateInfo =
		{
			memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			memoryAllocateInfo.pNext = nullptr,
			memoryAllocateInfo.allocationSize = memoryRequirements.size,
			memoryAllocateInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
		};
		
		vkAllocateMemory(logicalDevice, &memoryAllocateInfo, nullptr, &memoryObject);
		
		vkBindBufferMemory(logicalDevice, vertexBuffer, memoryObject, 0);
		
		void* data;
		vkMapMemory(logicalDevice, memoryObject, 0, vertexBufferCreateInfo.size, 0, &data);
		memcpy(data, positions, (size_t)vertexBufferCreateInfo.size);
		vkUnmapMemory(logicalDevice, memoryObject);
	}

	void Command::DestroyVertexBuffer()
	{
		vkDestroyBuffer(device->logicalDevice, vertexBuffer, nullptr);
	}

	uint32_t Command::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(device->physicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}
}
