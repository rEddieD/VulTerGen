#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>
#include <vector>
#include "Shader.h"
#include "RenderPass.h"

namespace VulTerGen
{
	class Pipeline
	{
	public:
		RenderPass* renderPass;
		VkPipeline graphicPipeline;
	private:
		VkDevice logicalDevice;
		VkSurfaceCapabilitiesKHR surfaceCapabilities;

		VkPipelineLayout pipelineLayout; //Pipeline layouts are used only in three situations--creating pipelines, binding descriptor sets, and updating push constants

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;

		std::vector<VkVertexInputBindingDescription> vertexInputBindingDescriptions;
		std::vector<VkVertexInputAttributeDescription> vertexInputAttributeDescriptions;

		VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo;
		VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo;

		VkViewport viewport;
		VkRect2D scissor;
		VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo;
		VkPipelineRasterizationStateCreateInfo pipelineRasterizerStateCreateInfo;
		VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo;
		VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo;
		VkPipelineColorBlendAttachmentState blendAttachmentState[1];
		VkGraphicsPipelineCreateInfo graphicPipelineCreateInfo;

		Shader* vertexShader;
		Shader* fragmentShader;

		std::vector<VkPipelineShaderStageCreateInfo> pipelineShaderStageCreateInfos;

	public:
		Pipeline(VkDevice logicalDevice, VkSurfaceFormatKHR surfaceFormat, VkSurfaceCapabilitiesKHR surfaceCapabilities);
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