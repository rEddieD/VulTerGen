#include "RenderPass.h"

namespace VulTerGen
{
	RenderPass::RenderPass(VkDevice logicalDevice, VkSurfaceFormatKHR surfaceFormat)
	{
		this->logicalDevice = logicalDevice;
		this->surfaceFormat = surfaceFormat;

		AddColorAttachment();
		AddSubpass();
		AddSubpassDependency();

		VkRenderPassCreateInfo renderPassCreateInfo = {
			renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
			renderPassCreateInfo.pNext = nullptr,
			renderPassCreateInfo.flags = 0,
			renderPassCreateInfo.attachmentCount = attachments.size(),
			renderPassCreateInfo.pAttachments = attachments.data(),
			renderPassCreateInfo.subpassCount = subpasses.size(),
			renderPassCreateInfo.pSubpasses = subpasses.data(),
			renderPassCreateInfo.dependencyCount = subpassDependencies.size(),
			renderPassCreateInfo.pDependencies = subpassDependencies.data()
		};

		vkCreateRenderPass(logicalDevice, &renderPassCreateInfo, nullptr, &renderPass);
	}

	RenderPass::~RenderPass()
	{
		if (renderPass != VK_NULL_HANDLE)
		{
			vkDestroyRenderPass(logicalDevice, renderPass, nullptr);
			renderPass = VK_NULL_HANDLE;
		}
	}

	void RenderPass::AddColorAttachment()
	{
		VkAttachmentDescription colorAttachment = {
			colorAttachment.flags = 0,
			colorAttachment.format = surfaceFormat.format,
			colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT,
			colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
			colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
			colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
			colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED, //Before the render pass
			colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR //Automatically transition to when the render pass finishes
		};

		attachments.push_back(colorAttachment);
	}

	void RenderPass::AddSubpass()
	{
		colorAttachmentRef = {
			colorAttachmentRef.attachment = 0,
			colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL // Vulkan automatically converts the attachment to this layout
		};

		VkSubpassDescription subpass = {
			subpass.flags = 0,
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
			subpass.inputAttachmentCount = 0, //Attachments that are read from a shader
			subpass.pInputAttachments = nullptr, //Attachments that are read from a shader
			subpass.colorAttachmentCount = 1,
			subpass.pColorAttachments = &colorAttachmentRef,
			subpass.pResolveAttachments = nullptr, //Attachments used for multisampling color attachments
			subpass.pDepthStencilAttachment = nullptr, //Attachment for depth and stencil data
			subpass.preserveAttachmentCount = 0, //Attachments that are not used by this subpass, but for which the data must be preserved
			subpass.pPreserveAttachments = nullptr //Attachments that are not used by this subpass, but for which the data must be preserved
		};

		subpasses.push_back(subpass);
	}

	void RenderPass::AddSubpassDependency()
	{
		VkSubpassDependency dependency = {
			dependency.srcSubpass = VK_SUBPASS_EXTERNAL,
			dependency.dstSubpass = 0,
			dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			dependency.srcAccessMask = 0,
			dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
			dependency.dependencyFlags = 0,
		};

		subpassDependencies.push_back(dependency);
	}
}
