#include "Command.h"

namespace VulTerGen
{
<<<<<<< HEAD
	Command::Command(Device* device, Swapchain* swapchain, Pipeline* pipeline, RenderPass* renderPass)
	{
		this->device = device;
		this->swapchain = swapchain;
		this->pipeline = pipeline;
		this->renderPass = renderPass;
		commandBuffers.resize(swapchain->swapchainFramebuffers.size());
		CreateCommandPool();
		AllocateCommandBufferFromPool();
=======
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
>>>>>>> origin/master
	}

	Command::~Command()
	{
<<<<<<< HEAD
=======
		DestroyVertexBuffer();
>>>>>>> origin/master
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
<<<<<<< HEAD
			commandPoolCreateInfo.queueFamilyIndex = swapchain->graphicQueueFamilyIndex
		};

		vkCreateCommandPool(device->logicalDevice, &commandPoolCreateInfo, nullptr, &commandPool);
=======
			commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex
		};

		vkCreateCommandPool(logicalDevice, &commandPoolCreateInfo, nullptr, &commandPool);
>>>>>>> origin/master
	}

	void Command::DestroyCommandPool()
	{
		if (commandPool != VK_NULL_HANDLE)
		{
<<<<<<< HEAD
			vkDestroyCommandPool(device->logicalDevice, commandPool, nullptr);
=======
			vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
>>>>>>> origin/master
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
<<<<<<< HEAD
			commandBufferAllocateInfo.commandBufferCount = (uint32_t)commandBuffers.size()
		};

		vkAllocateCommandBuffers(device->logicalDevice, &commandBufferAllocateInfo, commandBuffers.data());
=======
			commandBufferAllocateInfo.commandBufferCount = commandBuffers.size()
		};

		vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, commandBuffers.data());
>>>>>>> origin/master
	}

	void Command::DeallocateCommandBuffersFromPool()
	{
		if (commandBuffers.size() > 0)
		{
<<<<<<< HEAD
			vkFreeCommandBuffers(device->logicalDevice, commandPool, commandBuffers.size(), commandBuffers.data());
=======
			vkFreeCommandBuffers(logicalDevice, commandPool, commandBuffers.size(), commandBuffers.data());
>>>>>>> origin/master
			commandBuffers.clear();
		}
	}

<<<<<<< HEAD
	void Command::RecordCommandBuffer(VkBuffer vertexBuffer)
=======
	void Command::RecordCommandBuffer()
>>>>>>> origin/master
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

<<<<<<< HEAD
		for (uint32_t i = 0; i < swapchain->swapchainFramebuffers.size(); ++i)
=======
		for (uint32_t i = 0; i < swapchainFramebuffers.size(); ++i)
>>>>>>> origin/master
		{
			VkRenderPassBeginInfo renderPassInfo = {
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
				renderPassInfo.pNext = nullptr,
<<<<<<< HEAD
				renderPassInfo.renderPass = renderPass->renderPass,
				renderPassInfo.framebuffer = swapchain->swapchainFramebuffers[i],
				renderPassInfo.renderArea.offset = {0, 0},
				renderPassInfo.renderArea.extent = swapchain->surfaceCapabilities.currentExtent,
=======
				renderPassInfo.renderPass = renderPass,
				renderPassInfo.framebuffer = swapchainFramebuffers[i],
				renderPassInfo.renderArea.offset = {0, 0},
				renderPassInfo.renderArea.extent = surfaceCapabilities.currentExtent,
>>>>>>> origin/master
				renderPassInfo.clearValueCount = 1,
				renderPassInfo.pClearValues = &clearColor
			};

			vkBeginCommandBuffer(commandBuffers[i], &commandBufferBeginInfo);

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

<<<<<<< HEAD
			vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->graphicPipeline);

			VkBuffer vertexBuffers[] = { vertexBuffer };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
=======
			vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicPipeline);

			vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, 0);
>>>>>>> origin/master

			vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(commandBuffers[i]);

			vkEndCommandBuffer(commandBuffers[i]);
		}
	}
<<<<<<< HEAD
=======

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
>>>>>>> origin/master
}
