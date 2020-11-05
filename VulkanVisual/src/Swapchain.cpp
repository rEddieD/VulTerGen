#include "Swapchain.h"

namespace VulTerGen
{
	Swapchain::Swapchain(VkInstance instance, Device* device, HWND hWnd)
	{
		this->instance = instance;
		this->device = device;
		CreatePresentationSurface(hWnd);
		if (IsImagePresentationSupported())
		{
			CreateSwapchain();
			GetSwapchainImage();
			CreateImageViews();
		}
	}

	Swapchain::~Swapchain()
	{
		DestroyImageViews();
		DestroySwapchain();
	}

	void Swapchain::CreatePresentationSurface(HWND hWnd)
	{
		VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {
			surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
			surfaceCreateInfo.pNext = nullptr,
			surfaceCreateInfo.flags = 0,
			surfaceCreateInfo.hinstance = GetModuleHandle(nullptr),
			surfaceCreateInfo.hwnd = hWnd
		};

		vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, nullptr, &presentationSurface);
	}

	bool Swapchain::IsMailboxPresentationSupported()
	{
		uint32_t presentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device->physicalDevice, presentationSurface, &presentModeCount, nullptr);
		std::vector<VkPresentModeKHR> presentationModes(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device->physicalDevice, presentationSurface, &presentModeCount, &presentationModes[0]);

		for (auto& presentationMode : presentationModes)
		{
			if (presentationMode == VkPresentModeKHR::VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return true;
			}
		}
		return false;
	}

	bool Swapchain::IsImagePresentationSupported()
	{
		VkBool32 supported = VK_FALSE;
		for (const auto& queuefamily : device->queueFamilies)
		{
			vkGetPhysicalDeviceSurfaceSupportKHR(device->physicalDevice, queuefamily.queueFamilyIndex, presentationSurface, &supported);
			if (supported == VK_TRUE)
			{
				graphicQueueFamilyIndex = queuefamily.queueFamilyIndex;
				return supported;
			}
		}
		return supported;
	}

	void Swapchain::CreateSwapchain()
	{
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device->physicalDevice, presentationSurface, &surfaceCapabilities);

		uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device->physicalDevice, presentationSurface, &formatCount, nullptr);
		std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device->physicalDevice, presentationSurface, &formatCount, &surfaceFormats[0]);

		surfaceFormat = { surfaceFormats[0].format, surfaceFormats[0].colorSpace };

		VkSwapchainCreateInfoKHR swapchainCreateInfo = {
			swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			swapchainCreateInfo.pNext = nullptr,
			swapchainCreateInfo.flags = 0,
			swapchainCreateInfo.surface = presentationSurface,
			swapchainCreateInfo.minImageCount = surfaceCapabilities.minImageCount,
			//Select the default ones.
			swapchainCreateInfo.imageFormat = surfaceFormats[0].format,
			swapchainCreateInfo.imageColorSpace = surfaceFormats[0].colorSpace,
			swapchainCreateInfo.imageExtent = surfaceCapabilities.currentExtent,
			swapchainCreateInfo.imageArrayLayers = 1,
			swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
			swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
			swapchainCreateInfo.queueFamilyIndexCount = 0,
			swapchainCreateInfo.pQueueFamilyIndices = nullptr,
			swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform,
			swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			swapchainCreateInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR,
			swapchainCreateInfo.clipped = VK_TRUE,
			swapchainCreateInfo.oldSwapchain = nullptr
		};

		vkCreateSwapchainKHR(device->logicalDevice, &swapchainCreateInfo, nullptr, &swapchain);
	}

	void Swapchain::GetSwapchainImage()
	{
		uint32_t swapchainImageCount = 0;
		vkGetSwapchainImagesKHR(device->logicalDevice, swapchain, &swapchainImageCount, nullptr);
		swapchainImages.resize(swapchainImageCount);
		vkGetSwapchainImagesKHR(device->logicalDevice, swapchain, &swapchainImageCount, swapchainImages.data());
	}

	void Swapchain::CreateImageViews()
	{
		swapchainImageViews.resize(swapchainImages.size());

		for (uint32_t i = 0; i < swapchainImages.size(); ++i)
		{
			//Create ImageView attachments
			VkImageViewCreateInfo imageViewCreateInfo =
			{
				imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				imageViewCreateInfo.pNext = nullptr,
				imageViewCreateInfo.flags = 0,
				imageViewCreateInfo.image = swapchainImages[i],
				imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D,
				imageViewCreateInfo.format = surfaceFormat.format,
				imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY,
				imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY,
				imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY,
				imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY,
				imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				imageViewCreateInfo.subresourceRange.baseMipLevel = 0,
				imageViewCreateInfo.subresourceRange.levelCount = 1,
				imageViewCreateInfo.subresourceRange.baseArrayLayer = 0,
				imageViewCreateInfo.subresourceRange.layerCount = 1,
			};

			vkCreateImageView(device->logicalDevice, &imageViewCreateInfo, nullptr, &swapchainImageViews[i]);
		}
	}

	void Swapchain::DestroyImageViews()
	{
		for (auto& imageView : swapchainImageViews)
		{
			vkDestroyImageView(device->logicalDevice, imageView, nullptr);

		}
	}

	void Swapchain::CreateFramebuffer(VkRenderPass renderPass)
	{
		swapchainFramebuffers.resize(swapchainImages.size());

		for (uint32_t i = 0; i < swapchainImages.size(); ++i)
		{
			VkFramebufferCreateInfo framebufferCreateInfo = {
			framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
			framebufferCreateInfo.pNext = nullptr,
			framebufferCreateInfo.flags = 0,
			framebufferCreateInfo.renderPass = renderPass,
			framebufferCreateInfo.attachmentCount = 1,
			framebufferCreateInfo.pAttachments = &swapchainImageViews[i],
			framebufferCreateInfo.width = surfaceCapabilities.currentExtent.width,
			framebufferCreateInfo.height = surfaceCapabilities.currentExtent.height,
			framebufferCreateInfo.layers = 1
			};

			vkCreateFramebuffer(device->logicalDevice, &framebufferCreateInfo, nullptr, &swapchainFramebuffers[i]);
		}
	}

	void Swapchain::DestroyFramebuffer()
	{
		for (uint32_t i = 0; i < swapchainFramebuffers.size(); ++i)
		{
			vkDestroyFramebuffer(device->logicalDevice, swapchainFramebuffers[i], nullptr);
		}
	}

	VkSemaphore Swapchain::CreateSemaphores()
	{
		VkSemaphore semaphore;
		VkSemaphoreCreateInfo semaphoreCreateInfo =
		{
			semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
			semaphoreCreateInfo.pNext = nullptr,
			semaphoreCreateInfo.flags = 0
		};
		
		vkCreateSemaphore(device->logicalDevice, &semaphoreCreateInfo, nullptr, &semaphore);
		return semaphore;
	}

	void Swapchain::DestroySemaphores(VkSemaphore semaphore)
	{
		vkDestroySemaphore(device->logicalDevice, semaphore, nullptr);
		semaphore = VK_NULL_HANDLE;
	}

	uint32_t Swapchain::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(device->physicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}

	void Swapchain::DestroySwapchain()
	{
		if (swapchain != VK_NULL_HANDLE)
		{
			vkDestroySwapchainKHR(device->logicalDevice, swapchain, nullptr);
			swapchain = VK_NULL_HANDLE;
		}
	}

	void Swapchain::SetupDraw(std::vector<VkCommandBuffer>& commandBuffers)
	{
		imageAvailableSemaphore = CreateSemaphores();
		commandBufferFinishedSemaphore = CreateSemaphores();

		flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		submitInfo =
		{
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			submitInfo.pNext = nullptr,
			submitInfo.waitSemaphoreCount = 1,
			submitInfo.pWaitSemaphores = &imageAvailableSemaphore,
			submitInfo.pWaitDstStageMask = &flags,
			submitInfo.commandBufferCount = commandBuffers.size(),
			submitInfo.pCommandBuffers = commandBuffers.data(),
			submitInfo.signalSemaphoreCount = 1,
			submitInfo.pSignalSemaphores = &commandBufferFinishedSemaphore
		};
			
		presentInfo =
		{
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			presentInfo.pNext = nullptr,
			presentInfo.waitSemaphoreCount = 1,
			presentInfo.pWaitSemaphores = &commandBufferFinishedSemaphore,
			presentInfo.swapchainCount = 1,
			presentInfo.pSwapchains = &swapchain,
			presentInfo.pImageIndices = &swapchainImageIndex,
			presentInfo.pResults = nullptr
		};
	}

	void Swapchain::Draw(VkFence commandBufferExecutionFinished)
	{
		vkAcquireNextImageKHR(device->logicalDevice, swapchain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &swapchainImageIndex);

		//Wait previous actions
		vkQueueWaitIdle(device->mainGraphicQueue);

		vkQueueSubmit(device->mainGraphicQueue, 1, &submitInfo, commandBufferExecutionFinished);

		vkQueuePresentKHR(device->mainGraphicQueue, &presentInfo);
	}

	void Swapchain::EndDraw()
	{
		vkDeviceWaitIdle(device->logicalDevice);

		DestroySemaphores(commandBufferFinishedSemaphore);
		DestroySemaphores(imageAvailableSemaphore);
	}
}