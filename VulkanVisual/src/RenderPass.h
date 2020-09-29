#pragma once
#include <vector>
#include <vulkan/vulkan.hpp>
#include "VulkanFunctionPointers.h"
<<<<<<< HEAD
#include "Device.h"
#include "Swapchain.h"
=======
>>>>>>> origin/master

namespace VulTerGen
{
	class RenderPass
	{
<<<<<<< HEAD
		Device* device;
		Swapchain* swapchain;
=======
		VkDevice logicalDevice;
		VkSurfaceFormatKHR surfaceFormat;
>>>>>>> origin/master

		std::vector<VkAttachmentDescription> attachments;
		std::vector<VkSubpassDescription> subpasses;
		std::vector<VkSubpassDependency> subpassDependencies;
		VkAttachmentReference colorAttachmentRef;
	public:
<<<<<<< HEAD
		RenderPass(Device* device, Swapchain* swapchain);
=======
		RenderPass(VkDevice logicalDevice, VkSurfaceFormatKHR surfaceFormat);
>>>>>>> origin/master
		~RenderPass();

		void AddColorAttachment();
		void AddSubpass();
		void AddSubpassDependency();

		VkRenderPass renderPass;
	};
}