#pragma once
#include <vector>
#include <vulkan/vulkan.hpp>
#include "VulkanFunctionPointers.h"

namespace VulTerGen
{
	class RenderPass
	{
		VkDevice logicalDevice;
		VkSurfaceFormatKHR surfaceFormat;

		std::vector<VkAttachmentDescription> attachments;
		std::vector<VkSubpassDescription> subpasses;
		std::vector<VkSubpassDependency> subpassDependencies;
		VkAttachmentReference colorAttachmentRef;
	public:
		RenderPass(VkDevice logicalDevice, VkSurfaceFormatKHR surfaceFormat);
		~RenderPass();

		void AddColorAttachment();
		void AddSubpass();
		void AddSubpassDependency();

		VkRenderPass renderPass;
	};
}