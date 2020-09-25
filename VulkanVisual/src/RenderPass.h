#pragma once
#include <vector>
#include <vulkan/vulkan.hpp>
#include "VulkanFunctionPointers.h"
#include "Device.h"
#include "Swapchain.h"

namespace VulTerGen
{
	class RenderPass
	{
		Device* device;
		Swapchain* swapchain;

		std::vector<VkAttachmentDescription> attachments;
		std::vector<VkSubpassDescription> subpasses;
		std::vector<VkSubpassDependency> subpassDependencies;
		VkAttachmentReference colorAttachmentRef;
	public:
		RenderPass(Device* device, Swapchain* swapchain);
		~RenderPass();

		void AddColorAttachment();
		void AddSubpass();
		void AddSubpassDependency();

		VkRenderPass renderPass;
	};
}