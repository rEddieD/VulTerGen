#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>
#include <vector>
#include "Shader.h"
#include "Device.h"
#include "Swapchain.h"
#include "RenderPass.h"

namespace VulTerGen
{
	class Pipeline
	{
	public:
		VkPipeline graphicPipeline;
	private:
		Device* device;
		Swapchain* swapchain;
		RenderPass* renderPass;

		VkPipelineLayout pipelineLayout; //Pipeline layouts are used only in three situations--creating pipelines, binding descriptor sets, and updating push constants
		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;

		std::vector<VkPipelineShaderStageCreateInfo> pipelineShaderStageCreateInfos;

		std::vector<VkVertexInputBindingDescription> vertexInputBindingDescriptions;
		std::vector<VkVertexInputAttributeDescription> vertexInputAttributeDescriptions;

		VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo;
		VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo;

		VkViewport viewport;
		VkRect2D scissor;
		VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo;
		
		VkPipelineRasterizationStateCreateInfo pipelineRasterizerStateCreateInfo;
		VkPipelineColorBlendAttachmentState blendAttachmentState[1];
		VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo;
		VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo;
		
		VkGraphicsPipelineCreateInfo graphicPipelineCreateInfo;

	public:
		Pipeline(Device* device, Swapchain* swapchain, RenderPass* renderpass);
		~Pipeline();

		void AddVertexInputBindingDescription();
		void AddVertexInputAttributeDescription();

		void SetVertexInputState();
		void SetInputAssemblyState();
		void SetViewportState();
		void SetRasterizationState();
		void SetColorBlendState();
		void SetMultisampleState();

		void CreatePipelineLayout();
		void DestroyPipelineLayout();
		void CreateGraphicPipeline();
		void DestoryGraphicPipeline();
	};
}